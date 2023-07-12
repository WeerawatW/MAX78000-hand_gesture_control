#include "post_process.h"
#include "tft_utils.h"
#include "uart_IR.h"
#include "board.h"
#include "mxc_device.h"
#include "mxc_delay.h"

#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft_ssd2119.h"
#endif
#include "led.h"

#include "example_config.h"

#ifdef BOARD_EVKIT_V1
int font_1 = urw_gothic_13_grey_bg_white;
#endif

const int dims[NUM_SCALES] = {
    18, 9, 4, 2
}; // NUM_PRIORS_PER_AR = SQUARE(dims[0]) + SQUARE(dims[1]) + SQUARE(dims[2]) + SQUARE(dims[3])
const float scales[NUM_SCALES] = { 0.15f, 0.35f, 0.55f, 0.725f };
const float ars[NUM_ARS] = { 0.85f, 0.60f, 0.40f, 0.25f };

//Arrays to store model outputs
static int8_t prior_locs[LOC_DIM * NUM_PRIORS]; //(x, y, w, h)
static int8_t prior_cls[NUM_CLASSES * NUM_PRIORS];
static uint16_t prior_cls_softmax[NUM_CLASSES * NUM_PRIORS] = { 0 };

//NMS related arrays
static uint16_t nms_scores[NUM_CLASSES - 2][MAX_PRIORS];
static uint16_t nms_indices[NUM_CLASSES - 2][MAX_PRIORS];
static uint8_t nms_removed[NUM_CLASSES - 2][MAX_PRIORS] = { 0 };
static int num_nms_priors[NUM_CLASSES - 2] = { 0 };

int middle_X, middle_Y, num6 = 0;
int num, num1, num2, num3, num4, num5 = 0;
int num7, num8, num9 = 0;

int get_prior_idx(int ar_idx, int scale_idx, int rel_idx)
{
    int prior_idx = 0;

    for (int s = 0; s < scale_idx; ++s) {
        prior_idx += NUM_ARS * SQUARE(dims[s]);
    }

    prior_idx += NUM_ARS * rel_idx + ar_idx;
    return prior_idx;
}

void get_indices(int *ar_idx, int *scale_idx, int *rel_idx, int prior_idx)
{
    int s;

    int prior_count = 0;

    for (s = 0; s < NUM_SCALES; ++s) {
        prior_count += (NUM_ARS * SQUARE(dims[s]));

        if (prior_idx < prior_count) {
            *scale_idx = s;
            break;
        }
    }

    int in_scale_idx = prior_idx;

    for (s = 0; s < *scale_idx; ++s) {
        in_scale_idx -= (NUM_ARS * SQUARE(dims[s]));
    }

    *ar_idx = in_scale_idx % NUM_ARS;
    *rel_idx = in_scale_idx / NUM_ARS; //SQUARE(dims[*scale_idx]);
}

void softmax(void)
{
    int i, ch, calc_softmax;
    float sum;

    memset(prior_cls_softmax, 0, sizeof(prior_cls_softmax));

    for (i = 0; i < NUM_PRIORS; ++i) {
        sum = 0.;
        calc_softmax = 0;

        for (ch = 1; ch < (NUM_CLASSES - 1); ++ch) {
            if (prior_cls[i * NUM_CLASSES + ch] >= prior_cls[i * NUM_CLASSES]) {
                calc_softmax = 1;
                break;
            }
        }

        if (calc_softmax == 0) {
            continue;
        }

        for (ch = 0; ch < (NUM_CLASSES); ++ch) {
            sum += exp(prior_cls[i * NUM_CLASSES + ch] / 128.);
        }

        for (ch = 0; ch < (NUM_CLASSES); ++ch) {
            prior_cls_softmax[i * NUM_CLASSES + ch] =
                (uint16_t)(65536. * exp(prior_cls[i * NUM_CLASSES + ch] / 128.) / sum);
        }
    }
}

void get_prior_locs(void)
{
    int8_t *loc_addr = (int8_t *)0x50403000; 

    int ar_idx, scale_idx, rel_idx, prior_idx, prior_count;

    for (ar_idx = 0; ar_idx < NUM_ARS; ++ar_idx) {
        int8_t *loc_addr_temp = loc_addr;

        for (scale_idx = 0; scale_idx < NUM_SCALES; ++scale_idx) {
            prior_count = SQUARE(dims[scale_idx]);

            for (rel_idx = 0; rel_idx < prior_count; ++rel_idx) {
                prior_idx = get_prior_idx(ar_idx, scale_idx, rel_idx);
                memcpy(&prior_locs[LOC_DIM * prior_idx], loc_addr_temp, LOC_DIM);
                loc_addr_temp += LOC_DIM;
            }
        }

        loc_addr += 0x8000;
    }
}

void get_prior_cls(void)
{
    int8_t *cl_addr = (int8_t *)0x50803000; 

    int ar_idx, cl_idx, scale_idx, rel_idx, prior_idx, prior_count;

    for (ar_idx = 0; ar_idx < NUM_ARS; ++ar_idx) {
        for (cl_idx = 0; cl_idx < NUM_CLASSES; cl_idx += 4) {
            int8_t *cl_addr_temp = cl_addr;

            for (scale_idx = 0; scale_idx < NUM_SCALES; ++scale_idx) {
                prior_count = SQUARE(dims[scale_idx]);

                for (rel_idx = 0; rel_idx < prior_count; ++rel_idx) {
                    prior_idx = get_prior_idx(ar_idx, scale_idx, rel_idx);
                    memcpy(&prior_cls[NUM_CLASSES * prior_idx + cl_idx], cl_addr_temp, 4);
                    cl_addr_temp += 4;
                }
            }

            cl_addr += 0x8000; 

            if ((cl_addr == (int8_t *)0x50823000) || (cl_addr == (int8_t *)0x50c23000)) {
                cl_addr += 0x003e0000;  
            }
        }
    }

    softmax();
}

void get_priors(void)
{
    get_prior_locs();
    get_prior_cls();
}

float calculate_IOU(float *box1, float *box2)
{
    float x_left = MAX(box1[0], box2[0]);
    float y_top = MAX(box1[1], box2[1]);
    float x_right = MIN(box1[2], box2[2]);
    float y_bottom = MIN(box1[3], box2[3]);
    float intersection_area;

    if (x_right < x_left || y_bottom < y_top) {
        return 0.0;
    }

    intersection_area = (x_right - x_left) * (y_bottom - y_top);

    float box1_area = (box1[2] - box1[0]) * (box1[3] - box1[1]);
    float box2_area = (box2[2] - box2[0]) * (box2[3] - box2[1]);

    float iou = (float)(intersection_area) / (float)(box1_area + box2_area - intersection_area);

    return iou;
}

void get_cxcy(float *cxcy, int prior_idx)
{
    int i, scale_idx, ar_idx, rel_idx, cx, cy;

    get_indices(&ar_idx, &scale_idx, &rel_idx, prior_idx);

    cy = rel_idx / dims[scale_idx];
    cx = rel_idx % dims[scale_idx];
    cxcy[0] = (float)((float)(cx + 0.5) / dims[scale_idx]);
    cxcy[1] = (float)((float)(cy + 0.5) / dims[scale_idx]);
    cxcy[2] = scales[scale_idx] * sqrt(ars[ar_idx]);
    cxcy[3] = scales[scale_idx] / sqrt(ars[ar_idx]);

    for (i = 0; i < 4; ++i) {
        cxcy[i] = MAX(0.0, cxcy[i]);
        cxcy[i] = MIN(cxcy[i], 1.0);
    }
}

void gcxgcy_to_cxcy(float *cxcy, int prior_idx, float *priors_cxcy)
{
    float gcxgcy[4];

    for (int i = 0; i < 4; i++) {
        gcxgcy[i] = (float)prior_locs[4 * prior_idx + i] / 128.0;
    }

    cxcy[0] = priors_cxcy[0] + gcxgcy[0] * priors_cxcy[2] / 10;
    cxcy[1] = priors_cxcy[1] + gcxgcy[1] * priors_cxcy[3] / 10;
    cxcy[2] = exp(gcxgcy[2] / 5) * priors_cxcy[2];
    cxcy[3] = exp(gcxgcy[3] / 5) * priors_cxcy[3];
}

void cxcy_to_xy(float *xy, float *cxcy)
{
    xy[0] = cxcy[0] - cxcy[2] / 3;
    xy[1] = cxcy[1] - cxcy[3] / 3;
    xy[2] = cxcy[0] + cxcy[2] / 3;
    xy[3] = cxcy[1] + cxcy[3] / 3;

    if (xy[0] < 0)
	{
		xy[0] = 0;
	}
	else if (xy[0] > 1.0)
	{
		xy[0] = 0.99;
	}
	
	if (xy[1] < 0)
	{
		xy[1] = 0;
	}
	else if (xy[1] > 1.0)
	{
		xy[1] = 0.99;
	}
	
	if (xy[2] < 0)
	{
		xy[2] = 0;
	}
	else if (xy[2] > 1.0)
	{
		xy[2] = 0.99;
	}
	
	if (xy[3] < 0)
	{
		xy[3] = 0;
	}
	else if (xy[3] > 1.0)
	{
		xy[3] = 0.99;
	}

    float midpoint_x = (xy[0] + xy[2]) / 2;
    float midpoint_y = (xy[1] + xy[3]) / 2;
    xy[4] = midpoint_x;
    xy[5] = midpoint_y;
}

void insert_val(uint16_t val, uint16_t *arr, int arr_len, int idx)
{
    if (arr_len < MAX_PRIORS) {
        arr[arr_len] = arr[arr_len - 1];
    }

    for (int j = (arr_len - 1); j > idx; --j) {
        arr[j] = arr[j - 1];
    }

    arr[idx] = val;
}

void insert_idx(uint16_t val, uint16_t *arr, int arr_len, int idx)
{
    if (arr_len < MAX_PRIORS) {
        arr[arr_len] = arr[arr_len - 1];
    }

    for (int j = (arr_len - 1); j > idx; --j) {
        arr[j] = arr[j - 1];
    }

    arr[idx] = val;
}

void insert_nms_prior(uint16_t val, int idx, uint16_t *val_arr, uint16_t *idx_arr, int *arr_len)
{
    if ((*arr_len == 0) || ((val <= val_arr[*arr_len - 1]) && (*arr_len != MAX_PRIORS))) {
        val_arr[*arr_len] = val;
        idx_arr[*arr_len] = idx;
    } else {
        for (int i = 0; i < *arr_len; ++i) {
            if (val > val_arr[i]) {
                insert_val(val, val_arr, *arr_len, i);
                insert_idx(idx, idx_arr, *arr_len, i);
                break;
            }
        }
    }

    *arr_len = MIN((*arr_len + 1), MAX_PRIORS);
}

void reset_nms(void)
{
    for (int cl = 0; cl < NUM_CLASSES - 2; ++cl) {
        num_nms_priors[cl] = 0;

        for (int p_idx = 0; p_idx < MAX_PRIORS; ++p_idx) {
            nms_scores[cl][p_idx] = 0;
            nms_indices[cl][p_idx] = 0;
            nms_removed[cl][p_idx] = 0;
        }
    }
}

void nms(void)
{
    int prior_idx, class_idx, nms_idx1, nms_idx2, prior1_idx, prior2_idx;
    uint16_t cls_prob;
    float prior_cxcy1[4];
    float prior_cxcy2[4];
    float cxcy1[4];
    float cxcy2[4];
    float xy1[4];
    float xy2[4];

    reset_nms();

    for (prior_idx = 0; prior_idx < NUM_PRIORS; ++prior_idx) {
        for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
            cls_prob = prior_cls_softmax[prior_idx * NUM_CLASSES + class_idx + 1];

            if (cls_prob < MIN_CLASS_SCORE) {
                continue;
            }

            //num_nms_priors[class_idx] = insert_nms_prior(cls_prob, prior_idx, nms_scores[class_idx], nms_indices[class_idx], num_nms_priors[class_idx]);
            insert_nms_prior(cls_prob, prior_idx, nms_scores[class_idx], nms_indices[class_idx],
                             &num_nms_priors[class_idx]);

        }
    }

    for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
        for (nms_idx1 = 0; nms_idx1 < num_nms_priors[class_idx]; ++nms_idx1) {
            if (nms_removed[class_idx][nms_idx1] != 1 &&
                nms_idx1 != num_nms_priors[class_idx] - 1) {
                for (nms_idx2 = nms_idx1 + 1; nms_idx2 < num_nms_priors[class_idx]; ++nms_idx2) {
                    prior1_idx = nms_indices[class_idx][nms_idx1];
                    prior2_idx = nms_indices[class_idx][nms_idx2];

                    // TODO: Let's implement the box loc finding before this nested loop for 100 priors
                    get_cxcy(prior_cxcy1, prior1_idx);
                    get_cxcy(prior_cxcy2, prior2_idx);

                    gcxgcy_to_cxcy(cxcy1, prior1_idx, prior_cxcy1);
                    gcxgcy_to_cxcy(cxcy2, prior2_idx, prior_cxcy2);

                    cxcy_to_xy(xy1, cxcy1);
                    cxcy_to_xy(xy2, cxcy2);

                    float iou = calculate_IOU(xy1, xy2);

                    if (iou > MAX_ALLOWED_OVERLAP) {
                        nms_removed[class_idx][nms_idx2] = 1;
                    }
                }
            }
        }
    }
}

void localize_objects(void)
{
    float prior_cxcy[4];
    float cxcy[4];
    float xy[6];
    int class_idx, prior_idx, global_prior_idx;
    char buff[TFT_BUFF_SIZE];
    float midpoint_Y, midpoint_X;

    nms();

    for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
        for (prior_idx = 0; prior_idx < num_nms_priors[class_idx]; ++prior_idx) {
            if (nms_removed[class_idx][prior_idx] != 1) {
                global_prior_idx = nms_indices[class_idx][prior_idx];
                get_cxcy(prior_cxcy, global_prior_idx);
                gcxgcy_to_cxcy(cxcy, global_prior_idx, prior_cxcy);
                cxcy_to_xy(xy, cxcy);

                // printf("class: %d, prior_idx: %d, prior: %d, x1: %.2f, y1: %.2f, x2: %.2f, y2: "
                //        "%.2f \n",
                //        class_idx, prior_idx, global_prior_idx, xy[0], xy[1], xy[2], xy[3]);
   
                draw_obj_rect(xy, class_idx, SCALE);

                midpoint_X = xy[4];
                midpoint_Y = xy[5];

                // printf("midpoint_X: %.2f\n", xy[4]);
                // printf("midpoint_Y: %.2f\n", xy[5]);

 // *************************************** Dynamic *************************************************

                if(class_idx == 5){
                    num5 += 1;

                    if((midpoint_Y >= 0.38) && (midpoint_Y <= 0.62)){
                        if((midpoint_X >= 0.35) && (midpoint_X <= 0.71)){
                            middle_X = 1;
                            middle_Y = 1;
                            if(num5 == 2){
                                LED_On(LED1);
                                MXC_Delay(15000);
                                LED_Off(LED1);
                                uart_send_msg("S");
                                // printf("sound\n");
                            }
                        }
                    }
                    // printf("num5: %d\n", num5);
                    // if((midpoint_Y >= 0.38) && (midpoint_Y <= 0.62)){
                    //     middle_Y = 1;
                    //     // if(num5 == 2){
                    //     //     LED_On(LED1);
                    //     //     MXC_Delay(15000);
                    //     //     LED_Off(LED1);
                    //     //     uart_send_msg("S");
                    //     // }
                    // }

                    if(middle_Y == 1){
                        if(midpoint_Y <= 0.37){
                            LED_On(LED1);
                            MXC_Delay(15000);
                            LED_Off(LED1);
                            area_t area3 = {80, 189, 170, 240};
                            MXC_TFT_ClearArea(&area3, 4);
                            TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "UP"));
                            MXC_Delay(9000);
                            MXC_TFT_ClearArea(&area3, 4);
                            uart_send_msg("U");
                            middle_Y = 0;
                            middle_X = 0;
                            num5 = 0;
                        }
                    }

                    if(middle_Y == 1){
                        if(midpoint_Y >= 0.63){
                            LED_On(LED1);
                            MXC_Delay(15000);
                            LED_Off(LED1);
                            area_t area3 = {80, 189, 170, 240};
                            MXC_TFT_ClearArea(&area3, 4);
                            TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "DOWN"));
                            MXC_Delay(9000);
                            MXC_TFT_ClearArea(&area3, 4);
                            uart_send_msg("D");
                            middle_Y = 0;
                            middle_X = 0;
                            num5 = 0;
                        }
                    }
                
                    // if((midpoint_X >= 0.35) && (midpoint_X <= 0.73)){
                    //     middle_X = 1;
                    //     // if(num5 == 2){
                    //     //     LED_On(LED1);
                    //     //     MXC_Delay(15000);
                    //     //     LED_Off(LED1);
                    //     //     uart_send_msg("S");
                    //     // }
                    // }

                    if((midpoint_Y >= 0.39) && (midpoint_Y <= 0.69)){
                        if(middle_X == 1){
                            if((midpoint_X >= 0.72)){
                                LED_On(LED1);
                                MXC_Delay(15000);
                                LED_Off(LED1);
                                area_t area3 = {80, 189, 170, 240};
                                MXC_TFT_ClearArea(&area3, 4);
                                TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "LEFT"));
                                MXC_Delay(9000);
                                MXC_TFT_ClearArea(&area3, 4);
                                uart_send_msg("L");
                                middle_Y = 0;
                                middle_X = 0;
                                num5 = 0;
                            }
                        }

                        if(middle_X == 1){
                            if(midpoint_X <= 0.34){
                                LED_On(LED1);
                                MXC_Delay(15000);
                                LED_Off(LED1);
                                area_t area3 = {80, 189, 170, 240};
                                MXC_TFT_ClearArea(&area3, 4);
                                TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "RIGHT"));
                                MXC_Delay(90000);
                                MXC_TFT_ClearArea(&area3, 4);
                                uart_send_msg("R");
                                middle_Y = 0;
                                middle_X = 0;
                                num5 = 0;
                            }
                        }
                    }
                }

 //  ***************************************  END ************************************************

 //  *************************************** ON OFF **********************************************
                if(class_idx == 0){
                    num += 1;
                }

                if(num == 2){
                    LED_On(LED1);
                    MXC_Delay(15000);
                    LED_Off(LED1);
                    // uart_send_msg("S");
                }

                if((num == 3) || (num == 4)){
                    if(class_idx == 5){
                        LED_On(LED1);
                        MXC_Delay(15000);
                        LED_Off(LED1);
                        area_t area3 = {80, 189, 170, 240};
                        MXC_TFT_ClearArea(&area3, 4);
                        TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "ON")); 
                        MXC_Delay(90000);
                        MXC_TFT_ClearArea(&area3, 4);
                        uart_send_msg("O");
                        num += 4;
                        num1 = 0;
                        num2 = 0;
                        num3 = 0;
                        num4 = 0;
                        num5 += 4;
                        num6 = 0;
                        num7 = 0;
                        num8 = 0;
                        num9 = 0;
                        middle_X = 0;
                        middle_Y = 0;
                        break;
                    }

                    if(class_idx == 4){
                        LED_On(LED1);
                        MXC_Delay(15000);
                        LED_Off(LED1);
                        area_t area3 = {80, 189, 170, 240};
                        MXC_TFT_ClearArea(&area3, 4);
                        TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "Return")); 
                        MXC_Delay(90000);
                        MXC_TFT_ClearArea(&area3, 4);
                        uart_send_msg("E");
                        num += 4;
                        num1 = 0;
                        num2 = 0;
                        num3 = 0;
                        num4 += 4;
                        num5 = 0;
                        num6 = 0;
                        num7 = 0;
                        num8 = 0;
                        num9 = 0;
                        middle_X = 0;
                        middle_Y = 0;
                        break;
                    }
                }
 //  ***************************************  END ***********************************************

 //  ***************************************  STATICS  ******************************************
                
                // if(class_idx == 1){
                //     num1 += 1;
                // }
                // else if(class_idx == 2){
                //     num2 += 1;
                // }
                else if (class_idx == 3){
                    num3 += 1;
                }
                else if (class_idx == 4){
                    num4 += 1;
                }
                // else if (class_idx == 5){
                //     num5 += 1;
                // }
                else if (class_idx == 6){
                    num6 += 1;
                }
                else if (class_idx == 7){
                    num7 += 1;
                }
                else if (class_idx == 9){
                    num9 += 1;
                }
            
                if((num1 == 2) || (num2 == 2) || (num3 == 2) || (num4 == 2) || (num6 == 2) || (num7 == 2) || (num9 == 2)){
                    LED_On(LED1);
                    MXC_Delay(15000);
                    LED_Off(LED1);
                }

                if (num6 == 2){
                    // printf("OK\n");
                    LED_On(LED1);
                    MXC_Delay(15000);
                    LED_Off(LED1);
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "OK"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("K");  
                    num = 0;
                    num1 = 0;
                    num2 = 0;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 += 4;
                    num7 = 0;
                    num9 = 0;
                    break;
                }

                if (num9 == 2){
                    LED_On(LED1);
                    MXC_Delay(15000);
                    LED_Off(LED1);
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "Home"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("H");  
                    num = 0;
                    num1 = 0;
                    num2 = 0;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num8 = 0;
                    num9 += 4;
                    middle_X = 0;
                    middle_Y = 0;
                    break;
                }

                if (num == 2){
                    // printf("zero\n");
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "ZERO"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("0");  //  Zero number
                    num += 1;
                    num1 = 0;
                    num2 = 0;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num9 = 0;
                    break;
                }
                if (num1 == 2){
                    // printf("one\n");
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "ONE"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("1");  // One number
                    num = 0;
                    num1 += 4;
                    num2 = 0;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num9 = 0;
                    break;
                }
                else if (num2 == 2){
                    // printf("two\n");
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "TWO"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("2");  // Two number
                    num = 0;
                    num1 = 0;
                    num2 += 4;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num9 = 0;
                    break;
                }
                else if (num3 == 2){
                    // printf("three\n");
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "THREE"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("3");  // Three number
                    num = 0;
                    num1 = 0;
                    num2 = 0;
                    num3 += 4;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num9 = 0;
                    break;
                }
                else if (num4 == 2){
                    // printf("four\n");
                    area_t area3 = {80, 189, 170, 240};
                    MXC_TFT_ClearArea(&area3, 4);
                    TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "FOUR"));
                    MXC_Delay(90000);
                    MXC_TFT_ClearArea(&area3, 4);
                    uart_send_msg("4");  // Four number
                    num = 0;
                    num1 = 0;
                    num2 = 0;
                    num3 = 0;
                    num4 += 4;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num9 = 0;
                    break;
                }

                // if(num5 == 2){
                //     area_t area3 = {80, 189, 170, 240};
                //     MXC_TFT_ClearArea(&area3, 4);
                //     TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "FIVE"));
                //     MXC_Delay(90000);
                //     MXC_TFT_ClearArea(&area3, 4);
                //     uart_send_msg("5");  // Five number
                //     num = 0;
                //     num1 = 0;
                //     num2 = 0;
                //     num3 = 0;
                //     num4 = 0;
                //     num5 += 4;
                //     num6 = 0;
                //     num7 = 0;
                //     left1 = 0;
                //     up1 = 0;
                //     right1 = 0;
                //     down = 0;
                //     break;
                // }

                // if ((num5 == 1) && (num1 == 1)){
                //     // printf("six\n");
                //     LED_On(LED1);
                //     MXC_Delay(20000);
                //     LED_Off(LED1);
                //     area_t area3 = {80, 189, 170, 240};
                //     MXC_TFT_ClearArea(&area3, 4);
                //     TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "SIX"));
                //     MXC_Delay(90000);
                //     MXC_TFT_ClearArea(&area3, 4);
                //     uart_send_msg("6");  // six number
                //     num = 0;
                //     num1 = 0;
                //     num2 = 0;
                //     num3 = 0;
                //     num4 = 0;
                //     num5 = 0;
                //     num6 = 0;
                //     num7 = 0;
                //     break;
                // }

                // if ((num5 == 1) && (num2 == 1)){
                //     // printf("seven\n");
                //     LED_On(LED1);
                //     MXC_Delay(20000);
                //     LED_Off(LED1);
                //     area_t area3 = {80, 189, 170, 240};
                //     MXC_TFT_ClearArea(&area3, 4);
                //     TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "SEVEN"));
                //     MXC_Delay(90000);
                //     MXC_TFT_ClearArea(&area3, 4);
                //     uart_send_msg("7");  // seven number
                //     num = 0;
                //     num1 = 0;
                //     num2 += 4;
                //     num3 = 0;
                //     num4 = 0;
                //     num5 += 4;
                //     num6 = 0;
                //     num7 = 0;
                //     break;
                // }

                // if ((num5 == 1) && (num3 == 1)){
                //     // printf("eight\n");
                //     LED_On(LED1);
                //     MXC_Delay(20000);
                //     LED_Off(LED1);
                //     area_t area3 = {80, 189, 170, 240};
                //     MXC_TFT_ClearArea(&area3, 4);
                //     TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "EIGHT"));
                //     MXC_Delay(90000);
                //     MXC_TFT_ClearArea(&area3, 4);
                //     uart_send_msg("8");  // eight number
                //     num = 0;
                //     num1 = 0;
                //     num2 = 0;
                //     num3 += 4;
                //     num4 = 0;
                //     num5 += 4;
                //     num6 = 0;
                //     num7 = 0;
                //     break;
                // }

                // if ((num5 == 1) && (num4 == 1)){
                //     // printf("nine\n");
                //     LED_On(LED1);
                //     MXC_Delay(20000);
                //     LED_Off(LED1);
                //     area_t area3 = {80, 189, 170, 240};
                //     MXC_TFT_ClearArea(&area3, 4);
                //     TFT_Print(buff, 130, 210, snprintf(buff, sizeof(buff), "NINE"));
                //     MXC_Delay(90000);
                //     MXC_TFT_ClearArea(&area3, 4);
                //     uart_send_msg("9");  // nine number
                //     num = 0;
                //     num1 = 0;
                //     num2 = 0;
                //     num3 = 0;
                //     num4 += 4;
                //     num5 += 4;
                //     num6 = 0;
                //     num7 = 0;
                //     break;
                // }

                if((num >= 5) || (num1 >= 4) || (num2 >= 4) || (num3 >= 4) ||(num4 >= 4) || (num5 >= 4) || (num6 >= 4) || (num7 >= 4) || (num9 >= 4)){
                    num = 0;
                    num1 = 0;
                    num2 = 0;
                    num3 = 0;
                    num4 = 0;
                    num5 = 0;
                    num6 = 0;
                    num7 = 0;
                    num8 = 0;
                    num9 = 0;
                    break;
                }

            }
        }
    }
    
}


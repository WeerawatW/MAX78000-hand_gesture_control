#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft_ssd2119.h"
#endif

#include "example_config.h"

#define THICKNESS 2

void TFT_Print(char *str, int x, int y, int length);
void draw_obj_rect(float *xy, int class_idx, uint8_t scale);

import cv2
import os

images_path = "/home/max/Desktop/Max/06/9-06-2023/test_custom/train"
labels_path = "/home/max/Desktop/Max/06/9-06-2023/test_custom/processed/train_info.csv"
bbox_path = "/home/max/Desktop/Max/06/9-06-2023/test_custom/train"+"_Bounding_box_plot"

# images_path = "your images path"
# labels_path = "your label path (.csv format)"
# bbox_path = "your images path"+"_Bounding_box_plot"

i = 0
j = 0
count_img = 0
count_22 = 0
count_15 = 0
class_id, x, y, width, height = 0, 0, 0, 0, 0
try:
    os.mkdir(bbox_path)
except FileExistsError:
    pass
for image_file in os.listdir(images_path):
    #print(image_file)
    image = cv2.imread(images_path+'/'+image_file)
    with open (labels_path, 'r') as r:
            for data in r:
                    #print(data)
                    data_split  = data.split(',')
                    label_image = data_split[0]
                    #print(data_split)
                    #print(len(data_split))
                    if (image_file == label_image):
                        # print(image_file, "==", label_image)
                        
                        if len(data_split) == 15:
                            x0_1 =float(data_split[4].strip('"').strip('[').strip(']'))
                            y0_1 = float(data_split[5].strip('"').strip('[').strip(']'))
                            x1_1 = float(data_split[6].strip('"').strip('[').strip(']'))
                            y1_1 = float(data_split[7].strip('"').strip('[').strip(']'))
                            x0_1,y0_1,x1_1,y1_1 = int(x0_1),int(y0_1),int(x1_1),int(y1_1)
                            label_1 = data_split[1].strip('"').strip('[').strip(']')

                            image_height = image.shape[0]
                            image_width = image.shape[1]
                            width = x1_1 - x0_1
                            height = y1_1 - y0_1

                            start_point_1 = (x0_1, y0_1)
                            end_point_1 = (x1_1, y1_1)

                            color = (0, 255, 0)  
                            thickness = 3

                            cv2.rectangle(image, start_point_1, end_point_1, color, thickness)
                            cv2.putText(image,label_1,org= start_point_1,fontScale= 1,thickness= 2,fontFace=cv2.FONT_HERSHEY_SIMPLEX,color=(0,0,0))
                            cv2.imwrite(image_file[:-4]+"_with_bb"+".jpg", image)
                            print(image_file[:-4]+"_with_bb"+".jpg", "is saved!!\n")

                            count_15 += 1
                        elif len(data_split) == 22:
                            # print(data_split)
                            x0_1 = float(data_split[7].strip('"').strip('['))
                            x0_2 = float(data_split[8].strip('"').strip('[').strip(']').strip(' '))

                            y0_1 = float(data_split[9].strip('"').strip('['))
                            y0_2 = float(data_split[10].strip('"').strip('[').strip(']').strip(' '))

                            x1_1 = float(data_split[11].strip('"').strip('['))
                            x1_2 = float(data_split[12].strip('"').strip('[').strip(']').strip(' '))

                            y1_1 = float(data_split[13].strip('"').strip('['))
                            y1_2 = float(data_split[14].strip('"').strip('[').strip(']').strip(' '))

                            label_1 = data_split[1].strip('"').strip('[')
                            label_2 = data_split[2].strip('"').strip('[').strip(']').strip(' ')

                            x0_1,y0_1,x1_1,y1_1 = int(x0_1),int(y0_1),int(x1_1),int(y1_1)
                            x0_2,y0_2,x1_2,y1_2 = int(x0_2),int(y0_2),int(x1_2),int(y1_2)

                            # print(x0_1,y0_1,x1_1,y1_1)
                            # print(x0_2,y0_2,x1_2,y1_2)
                            count_22 += 1

                            image_height = image.shape[0]
                            image_width = image.shape[1]
                            width = x1_1 - x0_1
                            height = y1_1 - y0_1

                            # start point and end point 
                            start_point_1 = (x0_1, y0_1)
                            end_point_1 = (x1_1, y1_1)
                            start_point_2 = (x0_2, y0_2)
                            end_point_2 = (x1_2, y1_2)

                            # print(image_height,image_width)
                            # print(start_point_1,start_point_2)
                            # print(end_point_1,end_point_2)
                            # print(label_1, label_2)
                            
                            color = (0, 255, 0)  # green
                            thickness = 3 # thickness line
                            os.chdir(bbox_path)
                            # plot bounding box and label
                            cv2.rectangle(image, start_point_1, end_point_1, color, thickness)
                            cv2.putText(image,label_1,org= start_point_1,fontScale= 1,thickness= 2,fontFace=cv2.FONT_HERSHEY_SIMPLEX,color=(0,0,0))
                            cv2.rectangle(image, start_point_2, end_point_2, color, thickness)
                            cv2.putText(image,label_2,org = start_point_2,fontScale=1,thickness= 2,fontFace=cv2.FONT_HERSHEY_SIMPLEX,color=(255,0,0))
                            
                            cv2.imwrite(image_file[:-4]+"_with_bb"+".jpg", image)
                            print(image_file[:-4]+"_with_bb"+".jpg", "is saved!!\n")

print('1 object: %d , 2 object: %d , total_images: %d'%(count_15,count_22,count_15+count_22))
from PIL import Image
import os
import csv
import shutil

# You can path here
mode = 'train' # train or test
labels_path = '/home/max/Desktop/Max/06/9-06-2023/test_custom/'+mode+'_label/_annotations.txt'
image_path = '/home/max/Desktop/Max/06/9-06-2023/test_custom/'+mode
new_file_path ='/home/max/Desktop/Max/06/9-06-2023/test_custom/'+mode+'_label/'+mode+'_info.csv'
new_name_file = mode

# Or 
# mode = 'test' # train or test       # <------------------------------------- you can path here
# labels_path = 'your_text_path'      # <------------------------------------- you can path here
# image_path = 'your_image_path'      # <------------------------------------- you can path here
# new_file_path ='your_csv_path'      # <------------------------------------- you can path here
# new_name_file = mode    
#--------------------------------------------------------------------------------------------------

num_of_classes = 0
list_label = []
list_xmin = [] 
list_xmax = [] 
list_ymin = [] 
list_ymax = [] 
list_width = []
list_height = []
label_0 = 0
label_1 = 0
label_2 = 0
label_3 = 0
label_4 = 0
label_5 = 0
label_6 = 0
label_7 = 0
        
with open(new_file_path, "w") as f:
    f.write('img_name,label,width,height,x0,y0,x1,y1,num_of_boxes,img_width,img_height,bb_x0,bb_y0,bb_x1,bb_y1\n')
with open(labels_path,'r') as y:
    i = 0
    count_file = 0
    for data in y:
                #print(data)
                image_name_label = data.split()[0]
                for image_file in os.listdir(image_path):
                        image_name = image_file
                        if image_name_label == image_name:
                            print(image_name_label+' == '+image_name)
                            count_file += 1 
                            print(count_file)
                            try:
                                info = data.split()[1]
                                len_data = len(data.split())

                            except IndexError:
                                continue

#       1 object in 1 images                            
                            if len_data == 2:
                                info_split = info.split(',')
                                #print(info_split)
                                x0 = info_split[0]
                                y0 = info_split[1]
                                x1 = info_split[2]
                                y1 = info_split[3]
                                class_id_1 = info_split[4]
                                img = Image.open(os.path.join(image_path, image_name))
                                img_height = str(img.size[1])
                                img_width = str(img.size[0])
                                num_of_classes = 1
#--------------------------------- change label ---------------------------------- you can comment it if you don't want to change label
                                if class_id_1 == '0':
                                    class_id_1 = '1'

                                elif class_id_1 == '1':
                                    class_id_1 = '2'

                                elif class_id_1 == '2':
                                    class_id_1 = '3'

                                elif class_id_1 == '3':
                                    class_id_1 = '4'

                                elif class_id_1 == '4':
                                    class_id_1 = '5'

                                elif class_id_1 == '5':
                                    class_id_1 = '6'

                                elif class_id_1 == '6':
                                    class_id_1 = '7'

                                elif class_id_1 == '7':
                                    class_id_1 == '8'
#--------------------------------- change label --------------------------------
#--------------------------------- count ---------------------------------------
                                if class_id_1 == '0':
                                    label_0 += 1

                                elif class_id_1 == '1':
                                    label_1 += 1

                                elif class_id_1 == '2':
                                    label_2 += 1

                                elif class_id_1 == '3':
                                    label_3 += 1

                                elif class_id_1 == '4':
                                    label_4 += 1

                                elif class_id_1 == '5':
                                    label_5 += 1

                                elif class_id_1 == '6':
                                    label_6 += 1

                                elif class_id_1 == '7':
                                    label_7 += 1


                                #print(image_name,x0,y0,x1,y1,label)
                                list_label.append(class_id_1+'.0')
                                list_xmin.append(x0+'.0')
                                list_ymin.append(y0+'.0')
                                list_xmax.append(x1+'.0')
                                list_ymax.append(y1+'.0')
                                list_width.append(str(int(x1)-int(x0))+'.0')
                                list_height.append(str(int(y1)-int(y0))+'.0')
                                # print(' ')
                                # print("image_name_label",image_name_label)
                                
                                print("list_label",list_label)
                                print("list_xmin",list_xmin)
                                print("list_ymin",list_ymin)
                                print("list_xmax",list_xmax)
                                print("list_ymax",list_ymax)
                                print("list_width",list_width)
                                print("list_height",list_height)
#       2 object in 1 images
                            elif len(data.split()) == 3:
                                image_name_label = data.split()[0]
                                #print(image_name_label)
                                data_1 = data.split()[1]
                                data_2 = data.split()[2]
                                #print("data_1",data_1)
                                #print("data_2",data_2)
                                k = data_1.split(',')
                                b = data_2.split(',')
                                xmin_1 = k[0]
                                ymin_1 = k[1]
                                xmax_1 = k[2]
                                ymax_1 = k[3]
                                class_id_1 = k[4]
                                class_id_2 = b[4]
#--------------------------------- change label ---------------------------------- you can comment it if you don't want to change label
                                if class_id_1 == '0':
                                    class_id_1 = '1'

                                elif class_id_1 == '1':
                                    class_id_1 = '2'

                                elif class_id_1 == '2':
                                    class_id_1 = '3'

                                elif class_id_1 == '3':
                                    class_id_1 = '4'

                                elif class_id_1 == '4':
                                    class_id_1 = '5'

                                elif class_id_1 == '5':
                                    class_id_1 = '6'

                                elif class_id_2 == '6':
                                    class_id_2 = '7'

                                if class_id_2 == '0':
                                    class_id_2 = '1'

                                elif class_id_2 == '1':
                                    class_id_2 = '2'

                                elif class_id_2 == '2':
                                    class_id_2 = '3'

                                elif class_id_2 == '3':
                                    class_id_2 = '4'

                                elif class_id_2 == '4':
                                    class_id_2 = '5'

                                elif class_id_2 == '5':
                                    class_id_2 = '6'

                                elif class_id_2 == '6':
                                    class_id_2 = '7'

#--------------------------------- change label --------------------------------

# --------------------------------  count ----------------------------------------------
                                if class_id_1 == '0':
                                    label_0 += 1

                                elif class_id_1 == '1':
                                    label_1 += 1

                                elif class_id_1 == '2':
                                    label_2 += 1

                                elif class_id_1 == '3':
                                    label_3 += 1

                                elif class_id_1 == '4':
                                    label_4 += 1

                                elif class_id_1 == '5':
                                    label_5 += 1

                                elif class_id_1 == '6':
                                    label_6 += 1

                                elif class_id_1 == '7':
                                    label_7 += 1

                                
                                #print(xmin_1,ymin_1,xmax_1,ymax_1,class_id_1)
# --------------------------------- count -------------------------------------------------
                                xmin_2 = b[0]
                                ymin_2 = b[1]
                                xmax_2 = b[2]
                                ymax_2 = b[3]
                                
                                if class_id_2 == '0':
                                    label_0 += 1

                                elif class_id_2 == '1':
                                    label_1 += 1

                                elif class_id_2 == '2':
                                    label_2 += 1

                                elif class_id_2 == '3':
                                    label_3 += 1

                                elif class_id_2 == '4':
                                    label_4 += 1

                                elif class_id_2 == '5':
                                    label_5 += 1

                                elif class_id_2 == '6':
                                    label_6 += 1

                                elif class_id_2 == '7':
                                    label_7 += 1

                                
                                #print(xmin_2,ymin_2,xmax_2,ymax_2,class_id_2)

                                num_of_classes = 2
                                list_label.append(class_id_1+'.0')
                                list_label.append(class_id_2+'.0')
                                list_xmin.append(xmin_1+'.0')
                                list_xmin.append(xmin_2+'.0')
                                list_ymin.append(ymin_1+'.0')
                                list_ymin.append(ymin_2+'.0')
                                list_xmax.append(xmax_1+'.0')
                                list_xmax.append(xmax_2+'.0')
                                list_ymax.append(ymax_1+'.0')
                                list_ymax.append(ymax_2+'.0')
                                list_width.append(str(int(xmax_1)-int(xmin_1))+'.0')
                                list_width.append(str(int(xmax_2)-int(xmin_2))+'.0')
                                list_height.append(str(int(ymax_1)-int(ymin_1))+'.0')
                                list_height.append(str(int(ymax_2)-int(ymin_2))+'.0')

                            print("\nimage_name_label",image_name_label)
                            print("list_label",list_label)
                            print("list_xmin",list_xmin)
                            print("list_ymin",list_ymin)
                            print("list_xmax",list_xmax)
                            print("list_ymax",list_ymax)
                            print("list_width",list_width)
                            print("list_height",list_height)
                            img = Image.open(os.path.join(image_path, image_name))
                            img_height = str(img.size[1])
                            img_width = str(img.size[0])


# this code write csv only 2 object in 1 image!!, But you can add more object in 1 image
                            with open(new_file_path, "a+") as f:
                                i += 1
                                os.rename(os.path.join(image_path, image_file),os.path.join(image_path, new_name_file+'_'+str(i)+".jpg"))

                                try:
                                    new_name = new_name_file+'_'+str(i)+".jpg"
                                    f.writelines([new_name,',','\"'+str(list_label)+'\"',',','\"'+str(list_width)+'\"',',','\"'+str(list_height)+'\"',',','\"'+str(list_xmin)+'\"',',','\"'+str(list_ymin)+'\"',',','\"'+str(list_xmax)+'\"',',','\"'+str(list_ymax)+'\"',',',str(num_of_classes),',',img_width,',',img_height,',',str(min(list_xmin)),',',str(min(list_ymin)),',',str(max(list_xmax)),',',str(max(list_ymax)),'\n'])
                                except ValueError:
                                    continue

                                list_label.clear()
                                list_xmin.clear()
                                list_ymin.clear()
                                list_xmax.clear()
                                list_ymax.clear()
                                list_height.clear()
                                list_width.clear()
                            print("\nlabel_0_total = ", label_0)
                            print("label_1_total = ", label_1)
                            print("label_2_total = ", label_2)
                            print("label_3_total = ", label_3)
                            print("label_4_total = ", label_4)
                            print("label_5_total = ", label_5)
                            print("label_6_total = ", label_6)
                            print("label_7_total = ", label_7)
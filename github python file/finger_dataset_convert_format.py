from PIL import Image
import os
import csv
import shutil

# Example
mode = 'train' # train or test
path = '/home/max/Desktop/Max/SRM/SRM_shear/'
labels_path = path+mode+'/_annotations.txt'
new_label_path = path+mode+'_label'+'/_annotations.txt'
class_path = path+mode+'/_classes.txt'
image_path = path+mode
csv_path = path+mode+'_label/'+mode+'_info.csv'
new_file_name = 'SRM_'+mode+'_shear'

#you can path here.
# mode = 'train' # train or test  
# path = 'your_path'                                
# labels_path = path+mode+'_label/_annotations.txt'  
# class_path = path+mode+'/_classes.txt'          
# image_path = path+mode                                     
# csv_path = path+mode+'_label/'+mode+'_info.csv'        
# new_file_name = 'your_new_image_name'  
#----------------------------------- This file can convert maximum 2 object in 1 images -----------------------------------------

num_of_classes = 0
list_label = []
list_xmin = [] 
list_xmax = [] 
list_ymin = [] 
list_ymax = [] 
list_width = []
list_height = []
label_1 = 0
label_2 = 0
label_3 = 0
label_4 = 0
label_5 = 0
label_6 = 0
label_7 = 0
label_8 = 0
label_9 = 0
label_10 = 0

try:
    os.mkdir(image_path+'_rename')
    os.mkdir(path+mode+'_label')
except FileExistsError:
    pass

try:
    shutil.move(labels_path,path+mode+'_label')
    shutil.move(class_path,path+mode+'_label')
except FileNotFoundError:
    pass

with open(csv_path, "w") as f:
    f.write('img_name,label,width,height,x0,y0,x1,y1,num_of_boxes,img_width,img_height,bb_x0,bb_y0,bb_x1,bb_y1\n')
    
with open(new_label_path,'r') as y:
    i = 0
    for data in y:
                image_name_label = data.split()[0]
                for image_file in os.listdir(image_path):
                        image_name = image_file
                        if image_name_label == image_name:
                            print(" ")
                            print(image_name_label+' == '+image_name)
                            if len(data.split()) == 2:

                                data_1 = data.split()[1]
                                print(data_1)
                                k = data_1.split(',')
                                    
                                xmin = k[0]
                                ymin = k[1]
                                xmax = k[2]
                                ymax = k[3]
                                class_id = k[4]
                                # print(" ")
                                # print(class_id,end=' ')

#       1 object in 1 images                            
                            if len(data.split()) == 2:

                                data_1 = data.split()[1]
                                print("x0,y0,x1,y1,label")
                                print(data_1)
                                k = data_1.split(',')
                                    
                                xmin = k[0]
                                ymin = k[1]
                                xmax = k[2]
                                ymax = k[3]
                                class_id = k[4]
                                print(" ")
                                print(class_id,end=' ')

                                if class_id == '0':
                                    label_1 += 1
                                    class_id = '1'

                                elif class_id == '1':
                                    label_2 += 1
                                    class_id = '2'

                                elif class_id == '2':
                                    label_3 += 1
                                    class_id = '3'

                                elif class_id == '3':
                                    label_4 += 1
                                    class_id = '4'

                                elif class_id == '4':
                                    label_5 += 1
                                    class_id = '5'

                                elif class_id == '5':
                                    label_6 += 1
                                    class_id = '6'

                                elif class_id == '6':
                                    label_7 += 1
                                    class_id = '7'

                                elif class_id == '7':
                                    label_8 += 1
                                    class_id = '8'

                                elif class_id == '8':
                                    label_9 += 1
                                    class_id = '9'

                                elif class_id == '9':
                                    label_10 += 1
                                    class_id = '10'

                                print("convert to", class_id,'\n')
                                #print(image_name_label,xmin,ymin,xmax,ymax,class_id)
                                num_of_classes = 1
                                list_label.append(class_id+'.0')
                                list_xmin.append(xmin+'.0')
                                list_ymin.append(ymin+'.0')
                                list_xmax.append(xmax+'.0')
                                list_ymax.append(ymax+'.0')
                                list_width.append(str(int(xmax)-int(xmin))+'.0')
                                list_height.append(str(int(ymax)-int(ymin))+'.0')

#--------------------------------- change label ---------------------------------- you can comment it if you don't want to change label
#       2 object in 1 images  
                            elif len(data.split()) == 3:
                                image_name_label = data.split()[0]
                                #print(image_name_label)
                                data_1 = data.split()[1]
                                data_2 = data.split()[2]
                                print("x0,y0,x1,y1,label")
                                print("data_1",data_1)
                                print("x0,y0,x1,y1,label")
                                print("data_2",data_2)
                                k = data_1.split(',')
                                b = data_2.split(',')
                                xmin_1 = k[0]
                                ymin_1 = k[1]
                                xmax_1 = k[2]
                                ymax_1 = k[3]
                                class_id_1 = k[4]
                                #print(xmin_1,ymin_1,xmax_1,ymax_1,class_id_1)
                                xmin_2 = b[0]
                                ymin_2 = b[1]
                                xmax_2 = b[2]
                                ymax_2 = b[3]
                                class_id_2 = b[4]
                                #print(xmin_2,ymin_2,xmax_2,ymax_2,class_id_2)
                                num_of_classes = 2
                                print(class_id_1, "and" ,class_id_2,end=' ')
                                if class_id_1 == '0':
                                    class_id_1 = '1'
                                    label_1 += 1

                                elif class_id_1 == '1':
                                    class_id_1 = '2'
                                    label_2 += 1

                                elif class_id_1 == '2':
                                    class_id_1 = '3'
                                    label_3 += 1

                                elif class_id_1 == '3':
                                    class_id_1 = '4'
                                    label_4 += 1

                                elif class_id_1 == '4':
                                    class_id_1 = '5'
                                    label_5 += 1

                                elif class_id_1 == '5':
                                    class_id_1 = '6'
                                    label_6 += 1

                                elif class_id_1 == '6':
                                    class_id_1 = '7'
                                    label_7 += 1

                                elif class_id_1 == '7':
                                    class_id_1 = '8'
                                    label_8 += 1

                                elif class_id_1 == '8':
                                    class_id_1 == '9'
                                    label_9 += 1

                                elif class_id_1 == '9':
                                    class_id_1== '10'
                                    label_10 += 1
#------------------------------------------- Chnage label 2 -----------------------------------
                                if class_id_2 == '0':
                                    class_id_2 = '1'
                                    label_1 += 1

                                elif class_id_2 == '1':
                                    class_id_2 = '2'
                                    label_2 += 1

                                elif class_id_2 == '2':
                                    class_id_2 = '3'
                                    label_3 += 1

                                elif class_id_2 == '3':
                                    class_id_2 = '4'
                                    label_4 += 1

                                elif class_id_2 == '4':
                                    class_id_2 = '5'
                                    label_5 += 1

                                elif class_id_2 == '5':
                                    class_id_2 = '6'
                                    label_6 += 1

                                elif class_id_2 == '6':
                                    class_id_2 = '7'
                                    label_7 += 1

                                elif class_id_2 == '7':
                                    class_id_2 = '8'
                                    label_8 += 1

                                elif class_id_2 == '8':
                                    class_id_2 = '9'
                                    label_9 += 1

                                elif class_id_2 == '9':
                                    class_id_2 = '10'
                                    label_10 += 1

                                print('convert to ',class_id_1, 'and' ,class_id_2,'\n')
                                #print(image_name_label,xmin,ymin,xmax,ymax,class_id_1,class_id_2)
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

                            # print("image_name_label",image_name_label)
                            # print("list_label",list_label)
                            # print("list_xmin",list_xmin)
                            # print("list_ymin",list_ymin)
                            # print("list_xmax",list_xmax)
                            # print("list_ymax",list_ymax)
                            # print("list_width",list_width)
                            # print("list_height",list_height)

                            img = Image.open(os.path.join(image_path, image_name))
                            img_height = str(img.size[1])
                            img_width = str(img.size[0])

                            with open(csv_path, "a+") as f:
                                i += 1
                                shutil.copyfile(image_path+'/'+image_name, image_path+'_rename'+'/'+image_name)
                                os.rename(os.path.join(image_path+'_rename', image_file),os.path.join(image_path+'_rename', new_file_name+'_'+str(i)+".jpg"))
                                new_name = new_file_name+'_'+str(i)+".jpg"

                                try:
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

                            print("label_1_total = ", label_1)
                            print("label_2_total = ", label_2)
                            print("label_3_total = ", label_3)
                            print("label_4_total = ", label_4)
                            print("label_5_total = ", label_5)
                            print("label_6_total = ", label_6)
                            print("label_7_total = ", label_7)
                            print("label_8_total = ", label_8)
                            print("label_9_total = ", label_9)
                            print("label_10_total = ", label_10)

print("*-----------------------------------------------------*\nConvert dataset success!!")
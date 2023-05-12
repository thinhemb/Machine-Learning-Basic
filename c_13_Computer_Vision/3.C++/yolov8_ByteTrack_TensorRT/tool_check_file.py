import os

# Đường dẫn đến thư mục bạn muốn xóa các file
folder_path_label = "/home/thinhdo/Study/dev/data/labels"
folder_path_image="/home/thinhdo/Study/dev/data/image"
# Lặp qua các tệp trong thư mục
num=0
for filename in os.listdir(folder_path_image):
    file_path = os.path.join(folder_path_image, filename)
    path_txt=os.path.join(folder_path_label,filename.split('.')[0]+'.txt')
    if os.path.exists(path_txt):
        num+=1
        continue
    else:
        print(file_path)
        os.remove(file_path)
        print(path_txt)
print(num)
   
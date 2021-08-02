# Quy trình giải một bài toán ML

Bước 1: Load dữ liệu từ bộ nhớ vào trong RAM: cv2, os, numpy 

 * In shape của nó 

Bước 2: Mỗi cái ảnh sau khi được load thì trích đặc trưng (Feature Engineering) -> kích thước vector đặc trưng phải bằng nhau 

 * In shape của nó 

Bước 3: có x_train, y_train 

  * Xây dựng model 

  * Dựng model 

  * Khởi tạo.  clf = LinerSVC(max_inter, lr, )

  * W_new = w_cu  + lr * grad 
  * Train  clf.fit(X_train, y_train)

Bước 4: Đánh giá hiệu quả của mô hình clf.predict(X_test) 

  * so snahs cái predict với nhãn thực tế y_test -> acc, confusion matrix

Bưóc 5: cải tiến bằng cách thay đổi tham số của mô hình như số hidden layer, lr, tham số khác 


Bước 6: cải tiến trích chọn đặc trưng

  * Histogram, LBP, HOG, SIFT, SUFT, Canny, Lâpace Gause, Mặt Soble , LDZP, LBP Circle, GLCM 

-> H, đặc trưng mặt thống kê 
import cv2

# def camera(path_video):

def view_img(img):
    
    if platform.system() == 'Linux' and p not in windows:
        windows.append(p)
        cv2.namedWindow(str(p), cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # allow window resize (Linux)
        cv2.resizeWindow(str(p), img.shape[1], img.shape[0])
        cv2.imshow(str(p), img)
        cv2.waitKey(1)  # 1 millisecond
import cv2
import mediapipe as mp
import argparse
from pose_hands import Pose_hands
from models import GCN
import torch
from calc_fps import CvFpsCalc
from process_image import calc_bounding_rect,calc_landmark_list,\
                        draw_bounding_rect,draw_info,draw_info_text

    
def args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--no-cuda', action='store_true', default=True,
                        help='Disables CUDA training.')
    parser.add_argument('--fastmode', action='store_true', default=False,
                        help='Validate during training pass.')
    parser.add_argument('--seed', type=int, default=42,
                        help='Random seed.')
    
    parser.add_argument('--path_image', type=str, default="" , 
                        help="path image")
    parser.add_argument('--camera', type=bool, default=True, 
                        help="use Camera (default)")
    parser.add_argument('--video', type=str, default=" ", 
                        help=' path video ')
                        
    parser.add_argument('--model', type=str, default="best.pt",
                        help='path model.')
    parser.add_argument('--write_data',  action='store_true', default=False,
                        help='check write keypoint to data.json')
    parser.add_argument("--width", help='cap width', type=int, default=960)
    parser.add_argument("--height", help='cap height', type=int, default=540)
    args = parser.parse_args()
    
    return args
def process_image(image,model):
    process=Pose_hands(image)
    image,keypoint=process.keypoint_hands(draw_show=True)
    output=model(keypoint)
    image=draw(image,keypoint,output)
    cv2.imshow('MediaPipe Hands',image)


def detect(args):
    model= torch.load_state_dict(torch.load(args.model))
    cvFpsCalc = CvFpsCalc(buffer_len=10)
    print("=============== Start detect ===============")
    if args.camera ==False:
        if args.image !=" ":
            image=cv2.imread(args.image)
            process_image(image)
    else:
        if args.video!=" ":
            cap= cv2.VideoCapture(args.video)
        else:
            cap= cv2.Capture(0)
        while cap.IsOpen():
            fps = cvFpsCalc.get()
            i+=1
            if cv2.waitKey(5) & 0xFF == 27:
                break
            success, image= cap.read()
            if not success:
                print("Ignoring empty camera frame.")
                # If loading a video, use 'break' instead of 'continue'.
                continue
            # process_image(image,model)
        cap.release()
    

if __name__ =='__main__':
    args = args()
    detect(args)
    cv2.destroyAllWindows()
    print("=============== End detect ===============")
    
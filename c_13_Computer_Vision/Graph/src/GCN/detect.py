import cv2
import mediapipe as mp
import argparse
import Pose_hands
from models import GCN




def process_image(image):
    process=Pose_hands(image)
    image,keypoint=process.keypoint_hands(draw_show=True)
    output=model_gcn(keypoint)
    image=draw(image,keypoint,output)
    cv2.imshow('MediaPipe Hands',image)
    if cv2.waitKey(5) & 0xFF == 27:
        break
        
def draw(image,keypoint,output):
    # "draw bbox hands from keypoint on image"
    
    return image
                
            
    
    
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
                        help=' use Camera (default)")
    parser.add_argument('--video', type=str, default=" ", 
                        help=' path video ")
                        
    parser.add_argument('--model', type=str, default="best.pt",
                        help='path model.')
    parser.add_argument('--write_data',  action='store_true', default=False,
                        help='check write keypoint to data.json')
    args = parser.parse_args()
    args.cuda = not args.no_cuda and torch.cuda.is_available()
    return args

def detect(args):
    model_gcn= torch.load_state_dict(torch.load(self.args.model))
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
            i+=1
            success, image= cap.read()
            if not success:
                print("Ignoring empty camera frame.")
                # If loading a video, use 'break' instead of 'continue'.
                continue
            process_image(image)
        cap.release()
    cv2.destroyAllWindows()

if __name__ =='__main__':
    args = args()
    detect(args)
    print("=============== End detect ===============")
    
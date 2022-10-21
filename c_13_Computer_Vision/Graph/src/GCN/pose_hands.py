import cv2
import mediapipe as mp


class Pose_hands():
  
  def __init__(self,image):
    self.mp_drawing = mp.solutions.drawing_utils
    self.mp_drawing_styles = mp.solutions.drawing_styles
    self.mp_hands = mp.solutions.hands
    self.image=image
    self.results={}
  def keypoint_hands(self,draw_show=False,keypoint=True):
    with self.mp_hands.Hands(static_image_mode=True,max_num_hands=2,min_detection_confidence=0.8, min_tracking_confidence=0.5) as hands:
      self.image=cv2.flip(self.image,1) #lật ngược ảnh tu trai qua phai
      
      self.image.flags.writeable = False
      self.image=cv2.cvtColor(self.image, cv2.COLOR_BGR2RGB)
      self.results = hands.process(self.image)  
      
      self.image.flags.writeable = True   
      self.image=cv2.cvtColor(self.image, cv2.COLOR_RGB2BGR)
      if keypoint==True:
        try:
          h,w,c=self.image.shape
          
          for num,hand in enumerate(self.results.multi_hand_landmarks):
            if(draw_show==True):
              # Draw the hand annotations on the image.
              self.mp_drawing.draw_landmarks(self.image,hand,self.mp_hands.HAND_CONNECTIONS,
                                        self.mp_drawing_styles.get_default_hand_landmarks_style(),
                                        self.mp_drawing_styles.get_default_hand_connections_style())
            # for id, lm in enumerate(hand.landmark):
            #   cx,cy=int(lm.x*w),int(lm.y*h)
            #   print(id,cx,cy)
        except:
          pass
      return self.image
      
      



if __name__ == '__main__':

# For webcam input:
  i=0
  cap = cv2.VideoCapture("/home/thinhdo/Downloads/_DUBSTEP FINGERS_ _ Finger Tutting Hand Dance _ PNUT _ Skrillex Kill everybody.mp4")
  while cap.isOpened():
    i+=1
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      # If loading a video, use 'break' instead of 'continue'.
      continue
    if(i==1000):
      
      break

    process = Pose_hands(image)
    image=process.keypoint_hands(draw_show=True)
    
    # cv2.imshow('MediaPipe Hands', image)
    cv2.imshow('MediaPipe Hands',image)
    if cv2.waitKey(5) & 0xFF == 27:
      break
  cap.release()
  cv2.destroyAllWindows()
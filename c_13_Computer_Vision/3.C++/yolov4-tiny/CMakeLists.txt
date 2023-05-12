cmake_minimum_required(VERSION 3.16)
project(yolov4_tiny)

set(PROJECT libncnn.a)
link_directories(/home/thinhdo/WorkSpace/ncnn/build/install/lib)
include_directories(/home/thinhdo/WorkSpace/ncnn/build/install/include/ncnn)

include_directories(/home/thinhdo/opencv_build/include/include/opencv4)
link_directories(/home/thinhdo/opencv_build/include/lib/)


add_executable(yolov4_tiny 
    src/main.cpp 
    src/yolov4_tiny.cpp
    src/Hungarian.cpp
    src/KalmanTracker.cpp
    )
target_include_directories(yolov4_tiny PRIVATE
  src
  include
 )

target_link_libraries(yolov4_tiny 
    ${PROJECT}
    -lgomp
    -lopencv_core
    -lopencv_highgui
    -lopencv_imgproc
    -lopencv_imgcodecs
    -lopencv_videoio
    -lopencv_tracking
    -lopencv_video

)
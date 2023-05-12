cmake_minimum_required(VERSION 3.16)
project(bytetrack)

set(PROJECT libncnn.a)
link_directories(/home/thinhdo/ncnn/build/install/lib)
include_directories(/home/thinhdo/ncnn/build/install/include/ncnn)

include_directories(/home/thinhdo/opencv_build/include/include/opencv4)
link_directories(/home/thinhdo/opencv_build/include/lib/)





include_directories(/usr/local/include/eigen3)

add_executable(bytetrack 
    src/bytetrack.cpp 
    src/BYTETracker.cpp
    src/kalmanFilter.cpp
    src/lapjv.cpp
    src/STrack.cpp
    src/utils.cpp
    src/yolov4_tiny.cpp
    )
target_include_directories(bytetrack PRIVATE
  src
  include
 )

target_link_libraries( bytetrack
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
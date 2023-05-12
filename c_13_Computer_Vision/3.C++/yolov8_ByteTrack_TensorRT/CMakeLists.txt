cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_CUDA_ARCHITECTURES 60 61 62 70 72 75 86)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-11.8/bin/nvcc)

project(yolov8 LANGUAGES CXX CUDA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -g")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
option(CUDA_USE_STATIC_CUDA_RUNTIME ON)

# CUDA
find_package(CUDA REQUIRED)
message(STATUS "CUDA Libs: \n${CUDA_LIBRARIES}\n")
message(STATUS "CUDA Headers: \n${CUDA_INCLUDE_DIRS}\n")

# OpenCV
find_package(OpenCV REQUIRED)


# TensorRT
set(TensorRT_INCLUDE_DIRS /usr/local/TensorRT/include)
set(TensorRT_LIBRARIES /usr/local/TensorRT/lib)


message(STATUS "TensorRT Libs: \n${TensorRT_LIBRARIES}\n")
message(STATUS "TensorRT Headers: \n${TensorRT_INCLUDE_DIRS}\n")



include_directories(
        ${CUDA_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${TensorRT_INCLUDE_DIRS}
        ./include
        /usr/local/include/eigen3)

add_executable(${PROJECT_NAME}
        main.cpp
        BYTETracker.cpp
        STrack.cpp
        utils.cpp
        lapjv.cpp
        kalmanFilter.cpp
        include/yolov8.hpp
        include/common.hpp
        include/BYTETracker.h
        include/dataType.h
        include/kalmanFilter.h
        include/lapjv.h
        include/logging.h
        include/STrack.h
        )
        LINK_DIRECTORIES(
		lib/TensorRT/lib/
	)
# target_link_directories(
#         ${PROJECT_NAME} 
#         PUBLIC 
#         ${CUDA_LIBRARIES}
#         ${OpenCV_LIBRARIES}
#         ${TensorRT_LIBRARIES})

target_link_libraries(
        ${PROJECT_NAME} 
        PRIVATE 
        -lnvinfer 
        -lnvinfer_plugin 
        ${CUDA_LIBRARIES} 
        ${OpenCV_LIBS}
)

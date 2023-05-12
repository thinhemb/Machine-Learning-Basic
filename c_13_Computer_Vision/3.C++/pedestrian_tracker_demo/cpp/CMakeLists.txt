# Copyright (C) 2018-2019 Intel Corporation
# SPDX-License-Identifier: Apache-2.0
#

# file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# add_demo(NAME pedestrian_tracker_demo
#     SOURCES ${SOURCES}
#     HEADERS ${HEADERS}
#     INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
#     DEPENDENCIES monitors)

cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
project(PerTrack)

# find_package(OpenVINO REQUIRED)


link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/gflags/lib)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/pipelines/lib)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/monitors/lib)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/models/lib)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/utils/lib)

link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/opencv/lib)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/inference_engine/lib/intel64)
link_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/ngraph/lib)

include_directories(
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/utils/include
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/models/include
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/monitors/include
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/pipelines/include
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/gflags/include
)

include_directories(
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/ngraph/include
    /home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/inference_engine/include

    )

include_directories(/home/thinhdv/WorkSpace/pedestrian_tracker_demo/cpp/Libs/opencv/include)

include_directories(include)

file(GLOB SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC} main.cpp)

target_link_libraries(
    ${PROJECT_NAME} 
    -Wno-dev
    utils
    pipelines
    monitors
    models
    opencv_core opencv_highgui opencv_videoio opencv_imgproc opencv_imgcodecs opencv_gapi
    gflags_nothreads
    inference_engine
    ngraph
    )
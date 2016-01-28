project(opencv_test)
cmake_minimum_required(VERSION 2.8.4)
aux_source_directory(. SRC_LIST)

include_directories(../libs/opencv/include)
include_directories(../libs/boost/include)

set(opencv_lib_dir ../libs/opencv/lib)
#set(opencv_lib_dir /usr/local/lib)
#set(boost_lib_dir /usr/local/lib)
set(boost_lib_dir ../libs/boost/lib)
set(lib_suffix .so)

add_library(opencv_highgui SHARED IMPORTED)
set_property(TARGET opencv_highgui PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_highgui${lib_suffix})

add_library(opencv_objdetect SHARED IMPORTED)
set_property(TARGET opencv_objdetect PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_objdetect${lib_suffix})

add_library(opencv_imgproc SHARED IMPORTED)
set_property(TARGET opencv_imgproc PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_imgproc${lib_suffix})

add_library(opencv_features2d SHARED IMPORTED)
set_property(TARGET opencv_features2d PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_features2d${lib_suffix})

add_library(opencv_legacy SHARED IMPORTED)
set_property(TARGET opencv_legacy PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_legacy${lib_suffix})

add_library(opencv_nonfree SHARED IMPORTED)
set_property(TARGET opencv_nonfree PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_nonfree${lib_suffix})

add_library(opencv_core SHARED IMPORTED)
set_property(TARGET opencv_core PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_core${lib_suffix})

add_library(opencv_contrib SHARED IMPORTED)
set_property(TARGET opencv_contrib PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_contrib${lib_suffix})

add_library(opencv_flann SHARED IMPORTED)
set_property(TARGET opencv_flann PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_flann${lib_suffix})

add_library(opencv_calib3d SHARED IMPORTED)
set_property(TARGET opencv_calib3d PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_calib3d${lib_suffix})

add_library(opencv_ml SHARED IMPORTED)
set_property(TARGET opencv_ml PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/libopencv_ml${lib_suffix})

#add_library(lib_jpeg STATIC IMPORTED)
#set_property(TARGET lib_jpeg PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/liblibjpeg${lib_suffix})

#add_library(lib_png STATIC IMPORTED)
#set_property(TARGET lib_png PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/liblibpng${lib_suffix})

#add_library(lib_tiff STATIC IMPORTED)
#set_property(TARGET lib_tiff PROPERTY IMPORTED_LOCATION ${opencv_lib_dir}/liblibtiff${lib_suffix})

add_library(boost_regex SHARED IMPORTED)
set_property(TARGET boost_regex PROPERTY IMPORTED_LOCATION ${boost_lib_dir}/libboost_regex.so)

add_library(boost_filesystem SHARED IMPORTED)
set_property(TARGET boost_filesystem PROPERTY IMPORTED_LOCATION ${boost_lib_dir}/libboost_filesystem.so)

add_library(boost_system SHARED IMPORTED)
set_property(TARGET boost_system PROPERTY IMPORTED_LOCATION ${boost_lib_dir}/libboost_system.so)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} opencv_objdetect opencv_imgproc opencv_core opencv_highgui opencv_legacy opencv_nonfree
							opencv_contrib opencv_features2d opencv_flann opencv_calib3d opencv_ml boost_regex boost_filesystem boost_system)

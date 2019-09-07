cmake_minimum_required(VERSION 2.8.3)
project(ctrl)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  cv_bridge
  image_transport
  qt_build
  synchronizer
  thruster_controller
)


find_package(Qt4 REQUIRED COMPONENTS
  QtCore
  QtGui
)

find_package(OpenCV REQUIRED)

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
set(QT_MOC include/mainwindow.h)
SET(QT_SOURCES src/mainwindow.cpp)


QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include(
  ${QT_USE_FILE}
)



include_directories(
 include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
${QT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)


add_library(qt_lib ${QT_SOURCES} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(qt_lib ${QT_LIBRARIES} ${catkin_LIBRARIES} ${OPENCV_LIBS})

add_library(lib include/mainwindow.h src/mainwindow.cpp)
target_link_libraries(lib qt_lib ${OpenCV_LIBS})

add_executable(cont src/main.cpp)
target_link_libraries(cont lib ${OpenCV_LIBS})

add_executable(ct src/ctrl.cpp)
target_link_libraries(ct ${QT_LIBRARIES} ${catkin_LIBRARIES})

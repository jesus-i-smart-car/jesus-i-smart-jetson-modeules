cmake_minimum_required(VERSION 3.0.2)
project(xbox_joy_cmd_vel)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  sensor_msgs
)


catkin_package(
  INCLUDE_DIRS include
  LIBRARIES xbox_joy_cmd_vel
  CATKIN_DEPENDS geometry_msgs roscpp sensor_msgs
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(xbox_joy_cmd_vel_node src/xbox360_cmd_vel.cpp)
target_link_libraries(xbox_joy_cmd_vel_node ${catkin_LIBRARIES})

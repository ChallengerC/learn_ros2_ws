cmake_minimum_required(VERSION 3.8)
project(demo_turtle)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(base_interfaces_demo REQUIRED)

add_executable(demo_pub_sub src/demo_pub_sub.cpp)
ament_target_dependencies(
  demo_pub_sub
  "rclcpp"
  "turtlesim"
  "geometry_msgs"
)
install(TARGETS 
  demo_pub_sub
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_package()

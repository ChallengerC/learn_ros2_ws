cmake_minimum_required(VERSION 3.8)
project(learn_service_server)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(base_interfaces_demo REQUIRED)

# 添加 include 目录
include_directories(include)

# 构建可执行文件
add_executable(learn_service_server
  main.cpp
  src/LearnServer.cpp
)

# 链接依赖
ament_target_dependencies(learn_service_server
  rclcpp
  base_interfaces_demo
)

# 安装可执行文件
install(TARGETS learn_service_server
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

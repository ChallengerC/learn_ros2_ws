cmake_minimum_required(VERSION 3.8)
project(learn_topic)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(base_interfaces_demo REQUIRED)

# ------------------------------------------------------------
# 添加头文件目录
# ------------------------------------------------------------
include_directories(include)

# ------------------------------------------------------------
# 构建可执行文件
# ------------------------------------------------------------
add_executable(topic_publisher
  main.cpp
  src/TopicPublisher.cpp
)

# 链接依赖库
ament_target_dependencies(topic_publisher
  rclcpp
  std_msgs
  base_interfaces_demo
)

# ------------------------------------------------------------
# 安装规则
# ------------------------------------------------------------
install(TARGETS topic_publisher
  DESTINATION lib/${PROJECT_NAME}
)

# 安装头文件和 launch 等（如有）
install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

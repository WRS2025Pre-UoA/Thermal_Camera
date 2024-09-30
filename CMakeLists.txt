cmake_minimum_required(VERSION 3.5)
project(temperature)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)    # ROS2のC++用ライブラリ
find_package(sensor_msgs REQUIRED) # センサーメッセージを使う場合 (例)
find_package(std_msgs REQUIRED) # センサーメッセージを使う場合 (例)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories( ${MY_SOURCE_DIR}/src )
find_package( OpenCV REQUIRED )
find_package(cv_bridge REQUIRED)

# ソースファイルの指定と実行可能ファイルの生成
add_executable(temperature_node src/Image_publisher.cpp)
target_link_libraries(temperature_node ${OpenCV_LIBS} )

ament_target_dependencies(temperature_node rclcpp std_msgs sensor_msgs cv_bridge)

install(TARGETS temperature_node
  DESTINATION lib/${PROJECT_NAME})

# 依存パッケージをリンク
#ament_target_dependencies(temperature_node rclcpp sensor_msgs)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

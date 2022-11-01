cmake_minimum_required(VERSION 3.10)
# target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")


find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)


add_executable(run_stereo src/run_stereo.cpp)
target_link_libraries(run_stereo ${OpenCV_LIBS} Eigen3::Eigen stdc++fs)
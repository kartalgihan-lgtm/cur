cmake_minimum_required(VERSION 3.20)

project(dynamic_library_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(dynamic_library_client main.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(dynamic_library_client PRIVATE OpenSSL::SSL OpenSSL::Crypto)

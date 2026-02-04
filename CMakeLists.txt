cmake_minimum_required(VERSION 3.20)

project(cur LANGUAGES C CXX)

option(CUR_USE_STATIC_CURL "Link libcurl statically" OFF)
if (CUR_USE_STATIC_CURL)
  set(CURL_USE_STATIC_LIBS ON)
endif()

find_package(CURL REQUIRED)

add_executable(cur src/main.cpp)
target_compile_features(cur PRIVATE cxx_std_17)
target_link_libraries(cur PRIVATE CURL::libcurl)

if (CUR_USE_STATIC_CURL)
  target_compile_definitions(cur PRIVATE CURL_STATICLIB)
endif()

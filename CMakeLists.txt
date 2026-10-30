cmake_minimum_required(VERSION 3.22.1)

project(LeviSmooth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    preloader
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
)

FetchContent_MakeAvailable(preloader)

add_library(levi_smooth SHARED
    src/main.cpp
)

find_library(ANDROID_LOG_LIB log REQUIRED)

target_link_libraries(levi_smooth
    PRIVATE
    preloader
    ${ANDROID_LOG_LIB}
)

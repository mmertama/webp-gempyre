cmake_minimum_required(VERSION 3.21)

project(webp2png VERSION 1.0 LANGUAGES CXX)

#find_package(gempyre REQUIRED)
#find_package(WebP CONFIG REQUIRED) #  brew install webp
#set(WEBP_LIB WebP::webp WebP::libwebpmux)

add_executable(${PROJECT_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/webp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/webp.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

include(gempyre)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if (APPLE)
    if (UNIVESAL)
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Universal binary" FORCE)
    endif()  
endif()

target_link_libraries(${PROJECT_NAME} 
	PRIVATE gempyre::gempyre 
	PRIVATE $<TARGET_PROPERTY:webp_gempyre,LINK_LIBRARIES>)
    
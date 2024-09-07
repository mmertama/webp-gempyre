cmake_minimum_required(VERSION 3.21)

project(webp_gempyre VERSION 1.0 LANGUAGES CXX)

find_package(gempyre REQUIRED)
set(CMAKE_FIND_DEBUG_MODE TRUE)
find_package(WebP REQUIRED) #  brew install webp
set(WEBP_LIB WebP::webp WebP::libwebpmux WebP::webpdecoder WebP::webpdemux)

set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/webp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/webp.hpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(${PROJECT_NAME} PRIVATE src)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE gempyre::gempyre 
	PUBLIC ${WEBP_LIB})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})
install(FILES src/webp.hpp
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gempyre
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/webp_gempyreConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

add_subdirectory(test)

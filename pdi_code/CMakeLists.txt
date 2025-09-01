cmake_minimum_required(VERSION 3.10)
project(pdi_code)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(pdi_code app/run.cpp ${SOURCES})

target_link_libraries(pdi_code ${OpenCV_LIBS})

# Adicionar suporte para filesystem se necessário
if(CMAKE_CXX_STANDARD LESS 17)
    target_link_libraries(pdi_code stdc++fs)
endif()

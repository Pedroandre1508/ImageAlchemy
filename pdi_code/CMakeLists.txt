cmake_minimum_required(VERSION 3.10)
project(pdi_code)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

# Executável principal (M1 + M2.1)
add_executable(pdi_code app/run.cpp ${SOURCES})
target_link_libraries(pdi_code ${OpenCV_LIBS})

# Executável específico para M2.1
add_executable(pdi_m2 app/run_m2.cpp ${SOURCES})
target_link_libraries(pdi_m2 ${OpenCV_LIBS})

# Executável de comparação com OpenCV
add_executable(comparacao_opencv app/comparacao_opencv.cpp ${SOURCES})
target_link_libraries(comparacao_opencv ${OpenCV_LIBS})

# Adicionar suporte para filesystem se necessário
if(CMAKE_CXX_STANDARD LESS 17)
    target_link_libraries(pdi_code stdc++fs)
    target_link_libraries(pdi_m2 stdc++fs)
    target_link_libraries(comparacao_opencv stdc++fs)
endif()

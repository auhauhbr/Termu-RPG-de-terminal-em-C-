cmake_minimum_required(VERSION 3.16)
project(Termu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB TERMU_FONTES CONFIGURE_DEPENDS "src/*.cpp")
list(REMOVE_ITEM TERMU_FONTES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_library(termu_nucleo ${TERMU_FONTES})
target_include_directories(termu_nucleo PUBLIC include)

if(MSVC)
    target_compile_options(termu_nucleo PRIVATE /W4 /permissive-)
else()
    target_compile_options(termu_nucleo PRIVATE
        -Wall -Wextra -Wpedantic
        -finput-charset=UTF-8
        -fexec-charset=UTF-8
    )
endif()

add_executable(termu src/main.cpp)
target_link_libraries(termu PRIVATE termu_nucleo)
if(MSVC)
    target_compile_options(termu PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(termu PRIVATE -Wall -Wextra -Wpedantic
        -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

enable_testing()
add_executable(termu_testes tests/testes_regras.cpp)
target_link_libraries(termu_testes PRIVATE termu_nucleo)
if(MSVC)
    target_compile_options(termu_testes PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(termu_testes PRIVATE -Wall -Wextra -Wpedantic
        -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
add_test(NAME termu_regras COMMAND termu_testes)

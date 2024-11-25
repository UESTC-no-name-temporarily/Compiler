cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 设置C标准和C++标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# 定义源文件路径
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MAIN_SRC ${SRC_DIR}/main.c)
set(LEXER_SRC ${SRC_DIR}/lexer.l)
set(TEST_SRC ${CMAKE_SOURCE_DIR}/test.cpp)

# 添加可执行文件 main
add_executable(main ${MAIN_SRC})
add_custom_command(
    OUTPUT ${SRC_DIR}/lex.yy.c
    COMMAND flex ${LEXER_SRC}
    WORKING_DIRECTORY ${SRC_DIR}
    COMMENT "Generating lex.yy.c from lexer.l"
)
add_custom_target(
    generate_lex
    DEPENDS ${SRC_DIR}/lex.yy.c
)
add_dependencies(main generate_lex)
target_sources(main PRIVATE ${SRC_DIR}/lex.yy.c)
target_link_libraries(main fl)

# 添加可执行文件 test
add_executable(test ${TEST_SRC})

# 添加可执行文件 scan (带-DDEBUG_SCAN定义)
add_executable(scan ${SRC_DIR}/lex.yy.c)
add_dependencies(scan generate_lex)
target_compile_definitions(scan PRIVATE DEBUG_SCAN)
target_link_libraries(scan fl)

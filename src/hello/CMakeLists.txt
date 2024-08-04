project(hello)
#获取当前目录全部的cpp文件
file(GLOB HELLO_SOURCES "*.cpp")
add_library(${PROJECT_NAME} SHARED ${HELLO_SOURCES})
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${Web_DIR}
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    web
)

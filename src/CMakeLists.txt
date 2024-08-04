project(chat-server-main)
add_subdirectory(hello)
add_subdirectory(web)
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC 
    ${Hello_DIR}
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    hello
)


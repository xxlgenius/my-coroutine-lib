project(maintest)

add_executable(${PROJECT_NAME} maintest.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    ${Web_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    web
)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    ${Hello_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    hello
)

add_test(NAME mainfunctest COMMAND ${PROJECT_NAME})
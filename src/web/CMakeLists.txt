project(web)

file(GLOB WEB_SOURCES "*.cpp")
add_library(${PROJECT_NAME} SHARED ${WEB_SOURCES})



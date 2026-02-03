project(HttpServer VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_server
    src/main.cpp
    src/httpserver.h
    src/httpserver.cpp
    src/handlers/include/basehandler.h
    src/handlers/include/hello.h
    src/handlers/src/hello.cpp
    src/handlers/include/users.h
    src/handlers/src/users.cpp
    src/handlers/include/graph.h
    src/handlers/src/graph.cpp
    src/handlers/include/settings.h
    src/handlers/src/settings.cpp
    src/handlers/src/objects.cpp
    src/handlers/include/objects.h
    src/middleware/include/loggermiddleware.h
    src/middleware/src/loggermiddleware.cpp
    src/middleware/include/authmiddleware.h
    src/middleware/src/authmiddleware.cpp
)

target_link_libraries(http_server PRIVATE 
    qt_http_lib
    Qt6::Core 
    Qt6::Network
)

target_include_directories(http_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/middleware
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/app.json 
     DESTINATION ${CMAKE_BINARY_DIR})

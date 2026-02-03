project(HttpServer VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_server
    src/main.cpp
    src/httpserver.h
    src/httpserver.cpp
    src/handlers/include/basehandler.h
    src/handlers/include/hellohandler.h
    src/handlers/src/hellohandler.cpp
    src/handlers/include/usershandler.h
    src/handlers/src/usershandler.cpp
    src/handlers/include/graphhandler.h
    src/handlers/src/graphhandler.cpp
    src/handlers/include/settingshandler.h
    src/handlers/src/settingshandler.cpp
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

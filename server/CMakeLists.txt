project(HttpServer VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_server
    src/main.cpp
    src/httpserver.h
    src/httpserver.cpp
    src/handlers/basehandler.h
    src/handlers/hellohandler.h
    src/handlers/hellohandler.cpp
    src/handlers/usershandler.h
    src/handlers/usershandler.cpp
    src/handlers/graphhandler.h
    src/handlers/graphhandler.cpp
    src/handlers/settingshandler.h
    src/handlers/settingshandler.cpp
    src/middleware/loggermiddleware.h
    src/middleware/loggermiddleware.cpp
    src/middleware/authmiddleware.h
    src/middleware/authmiddleware.cpp
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

# Копирование конфигурации
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/config.json 
     DESTINATION ${CMAKE_BINARY_DIR}/server)

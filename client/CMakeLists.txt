project(HttpClient VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_client
    src/main.cpp
    src/httpclient.h
    src/httpclient.cpp
    src/requestbuilder.h
    src/requestbuilder.cpp
    src/models/user.h
    src/models/user.cpp
    src/models/graph.h
    src/models/graph.cpp
)

target_link_libraries(http_client PRIVATE 
    qt_http_lib
    Qt6::Core 
    Qt6::Network
)

target_include_directories(http_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

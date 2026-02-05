project(HttpClient VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_client
    src/main.cpp
    src/httpclient.hpp
    src/httpclient.cpp
    src/requestbuilder.hpp
    src/requestbuilder.cpp
    src/models/user.hpp
    src/models/user.cpp
    src/models/graph.hpp
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

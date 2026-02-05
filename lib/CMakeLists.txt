project(QtHttpLib VERSION 1.0.0 LANGUAGES CXX)

add_library(qt_http_lib STATIC
    requesthandler.hpp
    common/jsonutils.hpp
    common/jsonutils.cpp
    common/logger.hpp
    common/logger.cpp
    common/config.hpp
    common/config.cpp
)

target_link_libraries(qt_http_lib PRIVATE Qt6::Core Qt6::Network)

target_include_directories(qt_http_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
)

set_target_properties(qt_http_lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

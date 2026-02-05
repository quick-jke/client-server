project(HttpServer VERSION 1.0.0 LANGUAGES CXX)

add_executable(http_server
    src/main.cpp
    src/httpserver.hpp
    src/httpserver.cpp
    src/handlers/include/basehandler.hpp
    src/handlers/include/hello.hpp
    src/handlers/src/hello.cpp
    src/handlers/include/users.hpp
    src/handlers/src/users.cpp
    src/handlers/include/graph.hpp
    src/handlers/src/graph.cpp
    src/handlers/include/settings.hpp
    src/handlers/src/settings.cpp
    src/handlers/src/objects.cpp
    src/handlers/include/objects.hpp
    src/middleware/include/loggermiddleware.hpp
    src/middleware/src/loggermiddleware.cpp
    src/middleware/include/authmiddleware.hpp
    src/middleware/src/authmiddleware.cpp


    # src/store/include/filestr.hpp
    # src/store/include/ident.hpp
    # src/store/include/idnhash.hpp
    # src/store/include/indexprogress.hpp
    # src/store/include/mapstore.hpp
    # src/store/include/objdef.hpp
    # src/store/include/param.hpp
    # src/store/include/position_list.hpp
    # src/store/include/rcode.hpp
    # src/store/include/space_tree.hpp
    # src/store/include/tgdefs.hpp
    # src/store/include/tgstream.hpp
    # src/store/include/user_channel.hpp

    # src/store/src/space_tree.cpp
    # src/store/src/mapstore.cpp

)

target_link_libraries(http_server PRIVATE 
    qt_http_lib
    Qt6::Core 
    Qt6::Network
    Qt6::Core5Compat
)

target_include_directories(http_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/handlers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/middleware
    ${CMAKE_CURRENT_SOURCE_DIR}/src/store
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/app.json 
     DESTINATION ${CMAKE_BINARY_DIR})

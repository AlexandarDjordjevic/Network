cmake_minimum_required(VERSION 3.8)

set(this Network)

project(${this})

set(CMAKE_CXX_STANDARD 11)

set(Src 
    main.cpp
    src/Socket.cpp
    src/SocketEventManager.cpp
    src/Stream/Server.cpp
    src/Stream/Client.cpp
    src/Datagram/Server.cpp
    src/Datagram/Client.cpp
)

set(Include 
    include/Network/Socket.hpp
    include/Network/SocketEventManager.hpp
    include/Network/Stream/Server.hpp
    include/Network/Stream/Client.hpp
    include/Network/Datagram/Server.hpp
    include/Network/Datagram/Client.hpp
)

add_executable(${this} ${Src})

# add_library(${this} STATIC ${Src})

# set_target_properties(${this} PROPERTIES
#     FOLDER Libraries
# )

target_include_directories(${this} PUBLIC include)

target_link_libraries(${this} pthread)

# add_subdirectory(tests)


cmake_minimum_required(VERSION 3.10)
project(timp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(CryptoPP REQUIRED)

add_executable(server
    src/main.cpp
    src/Server.cpp
    src/ClientDB.cpp
    src/Authenticator.cpp
    src/VectorProcessor.cpp
    src/Logger.cpp
    src/UserInterface.cpp
)

target_link_libraries(server PRIVATE 
    ${Boost_LIBRARIES} 
    ${CRYPTOPP_LIBRARIES}
)

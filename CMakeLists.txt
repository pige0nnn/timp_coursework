cmake_minimum_required(VERSION 3.10)
project(timp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS program_options)
find_package(CryptoPP REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(server
    main.cpp
    Server.cpp
    ClientDB.cpp
    Authenticator.cpp
    VectorProcessor.cpp
    Logger.cpp
    UserInterface.cpp
)

target_link_libraries(server PRIVATE 
    ${Boost_LIBRARIES} 
    ${CRYPTOPP_LIBRARIES}
)

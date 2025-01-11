cmake_minimum_required(VERSION 3.30)
project(Networking)

set(CMAKE_CXX_STANDARD 23)

set(HEADER_FILES
        Include/Sockets.hpp
        Include/Address.hpp
        Source/Include/wsadata.hpp
        Include/tools/exception.hpp)

set(SOURCE_FILES Source/library.cpp
        Source/Sockets.cpp
        Source/Address.cpp
        Source/wsadata.cpp
        Source/tools/exception.cpp)

add_library(Networking STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Networking PRIVATE Source/Include)
target_include_directories(Networking PUBLIC Include)

option(TestProgram "Generate a test program along with the library" ON)

if (WIN32)
    target_link_libraries(Networking Ws2_32.lib)
endif ()

if (TestProgram)
    include(CTest)

    project(NetworkingTest)

    set(TEST_SOURCE_FILES NetworkingTest/Source/Main.cpp
            NetworkingTest/Source/Main.cpp)

    add_executable(NetworkingTest ${TEST_SOURCE_FILES})

    add_test(NAME UDPSocket COMMAND NetworkingTest)

    target_link_libraries(NetworkingTest PUBLIC Networking)
endif ()
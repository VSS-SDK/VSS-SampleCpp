PROJECT(vss-sample)
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11 -pthread")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
find_package(PkgConfig)
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)

include_directories(include include/Communications include/Interfaces)
include_directories(src src/Communications)

file(GLOB_RECURSE SRCS_CPP src/*.cpp)
file(GLOB_RECURSE SRCS_CC src/*.cc)
file(GLOB_RECURSE INCLUDE include/*.h)

set (VSS_CORE libvss-core.so)
link_directories(usr/lib)
include_directories(usr/local/include)

ADD_LIBRARY(vss-sample-core ${SRCS_CPP} ${SRCS_CC} ${INCLUDE})

target_include_directories(vss-sample-core PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(vss-sample-core PUBLIC ${ZeroMQ_LIBRARY})
TARGET_LINK_LIBRARIES(vss-sample-core ${VSS_CORE} ${PROTOBUF_LIBRARY})

ADD_EXECUTABLE(vss-sample src/Main.cpp)
TARGET_LINK_LIBRARIES(vss-sample vss-sample-core)

IF(RELEASE)
    SET(CMAKE_CXX_FLAGS "-Wall -Werror -o2 -std=c++11 -pthread")

    INCLUDE(CPack)

    INSTALL(TARGETS vss-sample DESTINATION bin)

    SET(CPACK_GENERATOR "DEB")
    SET(CPACK_DEBIAN_PACKAGE_NAME "vss-sample")
    SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "A simple way to send commands for robots on VSS-Simulator or real robots.")
    SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://sirlab.github.io/VSS-SDK/")
    SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Johnathan Fercher")
    SET(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
ELSE()
    INCLUDE_DIRECTORIES(third-party)

    enable_testing()

    add_subdirectory(third-party/googletest)

    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

    file(GLOB_RECURSE TEST test/*.cpp)

    ADD_EXECUTABLE(tests ${TEST})

    target_link_libraries(tests
            pthread
            gtest
            gtest_main
            vss-sample-core)

    SET(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++11 -pthread")
ENDIF()



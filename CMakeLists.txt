cmake_minimum_required(VERSION 3.14)

project(at_command_server CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(COMMON_SOURCES
    src/Config.cpp
    src/server/AtCommandDictionary.cpp
    src/server/AtCommandExecutor.cpp
    src/server/AtCommandResponseSender.cpp
    src/server/command_handlers/CopsCommandHandler.cpp
    src/server/command_handlers/CpinCommandHandler.cpp
    src/server/emu/ModemEmulator.cpp
)

add_executable(at_command_server
    src/main.cpp
    src/platform/TtyTerminal.cpp
    src/server/AtCommandServer.cpp
    ${COMMON_SOURCES}
)


add_executable(at_command_server_test
    tests/AtExecutorTest.cpp
    tests/TestAtCommandResponseSender.cpp
    ${COMMON_SOURCES}
)


target_link_libraries(
    at_command_server_test
    GTest::gtest_main
)

set (PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include
    src
)

target_include_directories(at_command_server PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES}
)

target_include_directories(at_command_server_test PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES}
)


include(GoogleTest)
gtest_discover_tests(at_command_server_test)

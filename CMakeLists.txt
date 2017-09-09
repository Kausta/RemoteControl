cmake_minimum_required(VERSION 3.7)

set(MY_PROJECT_NAME RemoteControl)
set(MY_PROJECT_VERSION 0.1)

project(${MY_PROJECT_NAME} VERSION ${MY_PROJECT_VERSION} LANGUAGES CXX)

# Set default warnings
set(WARNINGS_PRJ ${MY_PROJECT_NAME}-warnings)
add_library(${WARNINGS_PRJ} INTERFACE)
set(ARGS_PRJ ${MY_PROJECT_NAME}-args)
add_library(${ARGS_PRJ} INTERFACE)

if (MSVC)
    target_compile_options(${WARNINGS_PRJ} INTERFACE /W4)
    target_compile_options(${ARGS_PRJ} INTERFACE /D "_HAS_AUTO_PTR_ETC" /std:c++latest)
else ()
    target_compile_options(${WARNINGS_PRJ} INTERFACE -Wall -Wextra -Wpedantic -Werror)
    if (${CMAKE_VERSION} VERSION_LESS 3.8)
        target_compile_options(${ARGS_PRJ} INTERFACE --std=c++1z)
    endif ()
endif ()
function(SetCxxVersionFor target)
    if (NOT (${CMAKE_VERSION} VERSION_LESS 3.8))
        set_target_properties(${target} PROPERTIES CXX_STANDARD 17)
        message("cmake >= 3.8")
    endif ()

    set_target_properties(${target} PROPERTIES
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO)
endfunction()

# Set external libraries
set(EXT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Set Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS system program_options)

add_library(Boost_Lib INTERFACE)
target_include_directories(Boost_Lib SYSTEM INTERFACE ${Boost_INCLUDE_DIRS}) 
target_link_libraries(Boost_Lib INTERFACE ${Boost_LIBRARIES})
if (MSVC)
    target_compile_definitions(Boost_Lib INTERFACE -DBOOST_ALL_NO_LIB)
endif()

# Set Catch
set(CATCH_INCLUDE_DIR ${EXT_SOURCE_DIR}/catch/)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Add the library
set(MY_LIB_NAME ${MY_PROJECT_NAME}-lib)
set(SOURCE_FILES src/Program.cpp include/Program.h include/IncludeCore.h src/Command.cpp include/Command.h src/CommandManager.cpp include/CommandManager.h
        src/CommandReader.cpp include/CommandReader.h src/MouseControl.cpp include/MouseControl.h src/Server.cpp include/Server.h src/ServerConnection.cpp include/ServerConnection.h
        src/WinUtility.cpp include/WinUtility.h)
add_library(${MY_LIB_NAME} ${SOURCE_FILES})

target_include_directories(${MY_LIB_NAME} PRIVATE include)
target_link_libraries(${MY_LIB_NAME} PRIVATE ${WARNINGS_PRJ} ${ARGS_PRJ})
target_link_libraries(${MY_LIB_NAME} PUBLIC Boost_Lib)
if(WIN32)
    target_link_libraries(${MY_LIB_NAME} PUBLIC wsock32 ws2_32)
endif()

SetCxxVersionFor(${MY_LIB_NAME})

# Add the executable and link to the library part
add_executable(${MY_PROJECT_NAME} src/main.cpp)

target_link_libraries(${MY_PROJECT_NAME} PRIVATE ${WARNINGS_PRJ} ${ARGS_PRJ})
target_link_libraries(${MY_PROJECT_NAME} PRIVATE ${MY_LIB_NAME})

SetCxxVersionFor(${MY_PROJECT_NAME})

target_include_directories(${MY_PROJECT_NAME} PRIVATE include)

enable_testing()

set(TEST_PROJECT ${MY_PROJECT_NAME}-tests)

# Add the test project and link to the library part
set(TEST_SOURCE_FILES test/test_main.cpp)
add_executable(${TEST_PROJECT} ${TEST_SOURCE_FILES})

target_link_libraries(${TEST_PROJECT} PRIVATE ${WARNINGS_PRJ} ${ARGS_PRJ})
target_link_libraries(${TEST_PROJECT} PRIVATE Catch)
target_link_libraries(${TEST_PROJECT} PRIVATE ${MY_LIB_NAME})

SetCxxVersionFor(${TEST_PROJECT})

target_include_directories(${TEST_PROJECT} PRIVATE include)

#set(PARSE_CATCH_TESTS_VERBOSE ON)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${TEST_PROJECT})
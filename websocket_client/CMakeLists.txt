cmake_minimum_required(VERSION 3.15)
project(websocket_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Boost REQUIRED COMPONENTS system)
find_package(jsoncpp CONFIG REQUIRED)

# Add WebSocket++ include directory (installed by Homebrew)
if(EXISTS "/opt/homebrew/include")
    # M1/M2 Mac
    include_directories("/opt/homebrew/include")
    link_directories("/opt/homebrew/lib")
else()
    # Intel Mac
    include_directories("/usr/local/include")
    link_directories("/usr/local/lib")
endif()

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/main.cpp
    src/ws_client.cpp
    src/server_manager.cpp
)

# Add the executable
add_executable(ws_client ${SOURCES})

# Link libraries
target_link_libraries(ws_client PRIVATE
    Boost::system
    JsonCpp::JsonCpp
    pthread
)

# Add compiler flags
target_compile_options(ws_client PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# Add definitions for debug builds
target_compile_definitions(ws_client PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
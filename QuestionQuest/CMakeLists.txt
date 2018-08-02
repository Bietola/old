# initialize project
cmake_minimum_required(VERSION 3.5)
project(Proj VERSION 1.0.0 LANGUAGES CXX)

# load packages
find_package(Curses REQUIRED)

# create target executable
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/**/*.cpp)
add_executable(Proj ${PROJECT_SOURCES}) 

# include directories
include_directories(${CURSES_INCLUDE_DIR})
target_include_directories(Proj
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# compiler properties
target_compile_options(Proj PRIVATE -Wall)
target_compile_features(Proj PRIVATE cxx_std_17)

# export compilation database for YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# link libraries
target_link_libraries(Proj ${CURSES_LIBRARIES})

# copy resources from resource directories into build directory
set(source "${CMAKE_SOURCE_DIR}/assets")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
add_custom_command(
    TARGET Proj POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination} 
    DEPENDS ${destination}
    COMMENT "Created symbolic link to assets folder at ${source} to ${destination}"
)

# copy compile commands into root directory
set(source "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
set(destination "${CMAKE_SOURCE_DIR}/")
add_custom_command(
    TARGET Proj POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${source} ${destination}
    DEPENDS ${destination}
    COMMENT "Copied compilation database from ${source} to ${destination}"
)

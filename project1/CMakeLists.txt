cmake_minimum_required(VERSION 2.8)
# Set the project name variable to project1.
project(project1)

# Add your CMAKE_C_FLAGS AND CMAKE_SXX_FLAGS here
# You should include -Wall -Werror -Wshadow
# Make SURE to use -std=c11 and -std=c++11 where relevant.
set(CMAKE_C_FLAGS "-std=c11 -Wall -Wextra -Wshadow -Werror -g")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Werror -g")

# Add our include directory to CMake's search paths.
include_directories(${CMAKE_CURRENT_DIR}/include)

# Compile dyn_array_lib so we can use it.
add_library(dyn_array SHARED src/dyn_array.c)

# Compile the analysis executable.
add_executable(analysis src/analysis.c src/process_scheduling.c)

# link the dyn_array library we compiled against our analysis executable.
target_link_libraries(analysis dyn_array)

# Compile the the tester executable.
add_executable(${PROJECT_NAME}_test test/tests.cpp src/process_scheduling.c)

# Link ${PROJECT_NAME}_test with dyn_array and gtest and pthread libraries
target_link_libraries(${PROJECT_NAME}_test dyn_array gtest pthread)

enable_testing()
add_test(NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME}_test)

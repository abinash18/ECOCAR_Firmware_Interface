cmake_minimum_required(VERSION 3.0.0)
project(ECOCAR_FIRMWARE_INTERFACE VERSION 0.1.0)

include(CTest)
enable_testing()

# Here I add the required source and header files for my library.
# I can add more source files if needed here
add_library(
    CANBUS_INTERFACE SHARED # The library name is libCANBUS_INTERFACE and it is compiled as a shared object (.so)
    serial_interface.h
    serial_interface.cpp
)

add_executable(ECOCAR_FIRMWARE_INTERFACE main.cpp) 
# Link the serial library to my library
# Since i am compiling libCANBUS_INTERFACE as a seperate library i need to link
# any used ones into that rather than this project. since i am only using it for testing.
target_link_libraries(CANBUS_INTERFACE serialport) 
target_link_libraries(${PROJECT_NAME} CANBUS_INTERFACE)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

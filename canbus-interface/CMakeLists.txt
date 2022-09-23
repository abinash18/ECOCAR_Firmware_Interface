# Here I add the required source and header files for my library.
# I can add more source files if needed here
add_library(
    CANBUS_INTERFACE SHARED # The library name is libCANBUS_INTERFACE and it is compiled as a shared object (.so)
    serial_interface.h
    serial_interface.cpp
    cbi.h
    cbi.cpp
)

set_target_properties(CANBUS_INTERFACE PROPERTIES CMAKE_POSITION_INDEPENDENT_CODE ON)

# Link the serial library to my library
# Since i am compiling libCANBUS_INTERFACE as a seperate library i need to link
# any used ones into that rather than this project. since i am only using it for testing.
target_link_libraries(CANBUS_INTERFACE serialport)
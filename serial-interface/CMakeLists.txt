file(GLOB SOURCES *.cpp)
include_directories(src)

# Here I add the required source and header files for my library.
# I can add more source files if needed here
add_library(
    SERIAL_INTERFACE SHARED # The library name is libSERIAL_INTERFACE and it is compiled as a shared object (.so)
    ${SOURCES}
)

set_target_properties(SERIAL_INTERFACE PROPERTIES CMAKE_POSITION_INDEPENDENT_CODE ON)

# Link the serial library to my library
# Since i am compiling libCANBUS_INTERFACE as a seperate library i need to link
# any used ones into that rather than this project. since i am only using it for testing.
target_link_libraries(SERIAL_INTERFACE serialport)
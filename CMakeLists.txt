cmake_minimum_required(VERSION 3.0.0)
project(ECOCAR_FIRMWARE_INTERFACE VERSION 0.1.0)

# include(CTest)
# enable_testing()
add_subdirectory(serial-interface)

add_executable(ECOCAR_FIRMWARE_INTERFACE main.cpp)
set_target_properties(SERIAL_INTERFACE PROPERTIES CMAKE_POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} SERIAL_INTERFACE)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

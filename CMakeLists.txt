cmake_minimum_required(VERSION 3.0.0)
project(ECOCAR_FIRMWARE_INTERFACE VERSION 0.1.0)

# include(CTest)
# enable_testing()
add_subdirectory(canbus-interface)

add_executable(ECOCAR_FIRMWARE_INTERFACE main.cpp)

target_link_libraries(${PROJECT_NAME} CANBUS_INTERFACE)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

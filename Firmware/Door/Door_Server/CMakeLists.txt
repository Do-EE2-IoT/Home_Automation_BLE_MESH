# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/bluetooth/esp_ble_mesh/common_components/example_init
                         $ENV{IDF_PATH}/examples/bluetooth/esp_ble_mesh/common_components/example_nvs
                         ${CMAKE_CURRENT_LIST_DIR}/lib_for_esp32)

project(Door_Server)


#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "html/index.html" TEXT)



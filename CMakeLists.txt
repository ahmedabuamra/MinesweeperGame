cmake_minimum_required(VERSION 3.8)
project(MinesweeperGUI)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp GUIFunctions.cpp GUIFunctions.h functions.cpp functions.h)
add_executable(MinesweeperGUI ${SOURCE_FILES})


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules" ${CMAKE_MODULE_PATH}})
find_package(SFML 2 REQUIRED system window graphics network audio)

find_package(TGUI 0.7 REQUIRED)

target_link_libraries(MinesweeperGUI ${TGUI_LIBRARY} ${SFML_LIBRARIES})
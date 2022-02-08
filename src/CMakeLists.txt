# add sources to the executable
TARGET_SOURCES(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

include (${CMAKE_CURRENT_LIST_DIR}/message/CMakeLists.txt)
include (${CMAKE_CURRENT_LIST_DIR}/node/CMakeLists.txt)
include (${CMAKE_CURRENT_LIST_DIR}/server/CMakeLists.txt)
include (${CMAKE_CURRENT_LIST_DIR}/simulator/CMakeLists.txt)
include (${CMAKE_CURRENT_LIST_DIR}/utilities/CMakeLists.txt)

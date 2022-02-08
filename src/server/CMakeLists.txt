# add sources to the executable
TARGET_SOURCES(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/server.cpp
    ${CMAKE_CURRENT_LIST_DIR}/serverNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/taskManager.cpp
    )

include (${CMAKE_CURRENT_LIST_DIR}/database/CMakeLists.txt)
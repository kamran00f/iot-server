# add sources to the executable
TARGET_SOURCES(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/node.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nodeInterface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/nodeList.cpp
    )

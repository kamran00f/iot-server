# add sources to executable
TARGET_SOURCES(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/dnsUpdater.cpp
    ${CMAKE_CURRENT_LIST_DIR}/logger.cpp
    )
project(trlc_common)

add_library(common INTERFACE)
add_library(trlc::common ALIAS common)

target_include_directories(common INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
)

if(TRLC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(TRLC_ENUM_HEADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/")
    install(TARGETS common
        EXPORT trlcTargets # Export this target
        DESTINATION ${TRLC_LIBRARY_INSTALL_DIR} # Install location for libraries
        INCLUDES DESTINATION ${TRLC_INC_INSTALL_DIR} # Install location for headers
    )
    install(DIRECTORY "${TRLC_ENUM_HEADER_PATH}" # source directory
        DESTINATION ${TRLC_INC_INSTALL_DIR} # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.hpp" # select header files
    )
endif()

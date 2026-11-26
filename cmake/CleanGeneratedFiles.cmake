if(NOT IS_DIRECTORY "${NETWORK_BINARY_DIR}")
    message(FATAL_ERROR
        "Invalid build directory for cleanup: '${NETWORK_BINARY_DIR}'"
    )
endif()

file(GLOB generated_files
    "${NETWORK_BINARY_DIR}/compile_flags*.txt"
    "${NETWORK_BINARY_DIR}/*.map"
)
if(generated_files)
    file(REMOVE ${generated_files})
endif()

if(EXISTS "${REPORT_PATH}")
    configure_file("${REPORT_PATH}" "${REPORT_PATH}~" COPYONLY)
endif()

execute_process(
    COMMAND "${SIZE_TOOL}" ${SIZE_FILES}
    OUTPUT_FILE "${REPORT_PATH}"
    ERROR_VARIABLE size_error
    RESULT_VARIABLE size_result
)
if(NOT size_result EQUAL 0)
    message(FATAL_ERROR "size command failed: ${size_error}")
endif()

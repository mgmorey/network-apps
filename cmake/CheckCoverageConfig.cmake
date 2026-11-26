if(NOT "${COVERAGE_CONFIG}" STREQUAL "Debug" AND
   NOT "${COVERAGE_CONFIG}" STREQUAL "Default")
    message(FATAL_ERROR
        "The coverage target requires the Debug or Default configuration; "
        "selected '${COVERAGE_CONFIG}'."
    )
endif()

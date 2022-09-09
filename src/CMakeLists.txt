set(TARGET ${PROJECT_NAME}.app)

# Source files
set(SOURCE_FILES
    main.cpp
)

# Include directories
include_directories(
    ../include/
)

add_executable(${TARGET} ${SOURCE_FILES})

# Link library
# target_link_libraries(${TARGET}

# )
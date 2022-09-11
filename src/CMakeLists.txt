set(TARGET ${PROJECT_NAME}.app)

# Source files
set(SOURCE_FILES
    # Main
    main.cpp

    # Simulation
    simulation/simulation.cpp

    # Entities
    entities/entity.cpp
    entities/tile.cpp
    entities/bob.cpp
    entities/food.cpp

    # Utils
    utils/entity_utils.cpp
    utils/rng.cpp

    # Display
    display/console_display.cpp
)

add_executable(${TARGET} ${SOURCE_FILES})

# Link library
# target_link_libraries(${TARGET}

# )
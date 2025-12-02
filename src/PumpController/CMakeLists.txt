# Create pump controller library
add_library(controller)
add_library(pump_control::controller ALIAS controller)

# Public include directory
target_include_directories(controller PUBLIC
    include
)

# Source files
target_sources(controller PRIVATE
    src/PumpController.cpp
)

# Link dependencies this module uses
target_link_libraries(controller PUBLIC
    pump_control::ui         # uses Motor, Buttons, etc.
    pump_control::utils      # uses mapf()
)

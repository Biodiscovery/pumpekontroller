
add_library(utils)
add_library(pump_control::utils ALIAS utils)

# target_link_libraries(utils PRIVATE
    
# )

# target_link_libraries(utils PUBLIC
    
# )

target_include_directories(utils PUBLIC include)

target_sources(utils PRIVATE
    include/Utils/Math.hpp
    src/Math.cpp
)

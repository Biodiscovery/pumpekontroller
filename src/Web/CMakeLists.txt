add_library(web INTERFACE)

add_library(pump_control::web ALIAS web)

target_sources(web PRIVATE
    src/WebServer.cpp
)

target_include_directories(web INTERFACE
    include
)

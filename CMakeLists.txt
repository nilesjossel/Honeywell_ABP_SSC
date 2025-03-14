add_library(Honeywell_ABP
    Honeywell_SSC.cpp
)

target_include_directories(Honeywell_ABP PUBLIC 
    include
)

target_link_libraries(Honeywell_ABP
    pico_stdlib
    hardware_i2c
)
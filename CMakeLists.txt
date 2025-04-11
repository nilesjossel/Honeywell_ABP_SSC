add_library(Honeywell_ABP_SSC
    src/Honeywell_SSC.cpp
)

target_include_directories(Honeywell_ABP_SSC PUBLIC 
    include
)

target_link_libraries(Honeywell_ABP_SSC
    pico_stdlib
    hardware_i2c
)
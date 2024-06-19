project(LiteROV)

add_executable(LiteROV
    src/LiteROV.cpp
    src/SERVO.cpp
    src/JOYC.cpp
    )

target_include_directories(LiteROV PUBLIC
	inc
)

set(CMAKE_CXX_STANDARD 17)

# pull in common dependencies
target_link_libraries(LiteROV
	pico_stdlib
	hardware_i2c
	hardware_pwm
	)

# create map/bin/hex file etc.
pico_add_extra_outputs(LiteROV)

# add url via pico_set_program_url
example_auto_set_url(LiteROV)

# enable usb output, disable uart output
pico_enable_stdio_usb(LiteROV 1)
pico_enable_stdio_uart(LiteROV 0)

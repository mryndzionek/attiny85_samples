include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_avr_executable(
    attiny85
    main.c
	i2cmaster.S
	i2clcd.c
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_avr_executable(
    attiny85
    main.c
	i2cmaster.S
	i2clcd.c
)

add_avr_executable(
	mq_6
	mq_6.c
	i2cmaster.S
	i2clcd.c
)

add_avr_executable(
    lp_blink
    lp_blink.c
)

add_avr_executable(
    whisper
    annoyatron.c
    snd/snd_whisper.c
)

add_avr_executable(
    cricket
    annoyatron.c
    snd/snd_cricket.c
)

add_avr_executable(
    droplet
    annoyatron.c
    snd/snd_droplet.c
)

add_avr_executable(
    water
    annoyatron.c
    snd/snd_water.c
)

add_avr_executable(
    scifi
    annoyatron.c
    snd/snd_scifi.c
)

add_avr_executable(
    insect
    annoyatron.c
    snd/snd_insect.c
)

add_avr_executable(
    charlie
    charlie.c
)

add_avr_executable(
    geiger
    geiger.c
)


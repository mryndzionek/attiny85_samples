include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
avr_add_executable(
    attiny85
    main.c
	i2cmaster.S
	i2clcd.c
)

avr_add_executable(
	mq_6
	mq_6.c
	i2cmaster.S
	i2clcd.c
)

avr_add_executable(
    lp_blink
    lp_blink.c
)

avr_add_executable(
    whisper
    annoyatron.c
    snd/snd_whisper.c
)

avr_add_executable(
    cricket
    annoyatron.c
    snd/snd_cricket.c
)

avr_add_executable(
    droplet
    annoyatron.c
    snd/snd_droplet.c
)

avr_add_executable(
    water
    annoyatron.c
    snd/snd_water.c
)

avr_add_executable(
    scifi
    annoyatron.c
    snd/snd_scifi.c
)

avr_add_executable(
    insect
    annoyatron.c
    snd/snd_insect.c
)

avr_add_executable(
    charlie
    charlie.c
)

avr_add_executable(
    geiger
    geiger.c
)

file(GLOB U8G2_SRCS
     "../external/u8g2/csrc/*.c"
)

include_directories("../external/u8g2/csrc/")

avr_add_library(
    u8g2
    ${U8G2_SRCS}
)

avr_add_executable(
    u8g2_i2c_test
    i2c_master_usi.c
    u8x8_avr.c
    u8g2_i2c_test.c
)

avr_target_link_libraries(u8g2_i2c_test u8g2)


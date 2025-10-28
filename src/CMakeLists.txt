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

if(${AVR_MCU} STREQUAL "attiny85")
    avr_add_executable(
        whisper
        i2c_master_usi.c
        annoyatron.c
        snd/snd_whisper.c
    )

    avr_add_executable(
        cricket
        i2c_master_usi.c
        annoyatron.c
        snd/snd_cricket.c
    )

    avr_add_executable(
        critters
        i2c_master_usi.c
        annoyatron.c
        snd/snd_critters.c
    )

    avr_add_executable(
        droplet
        i2c_master_usi.c
        annoyatron.c
        snd/snd_droplet.c
    )

    avr_add_executable(
        water
        i2c_master_usi.c
        annoyatron.c
        snd/snd_water.c
    )

    avr_add_executable(
        scifi
        i2c_master_usi.c
        annoyatron.c
        snd/snd_scifi.c
    )

    avr_add_executable(
        insect
        i2c_master_usi.c
        annoyatron.c
        snd/snd_insect.c
    )

    avr_add_executable(
        speech
        speech.c
        speech_synth.c
    )

    avr_add_executable(
        ws2812
        light_ws2812.c
        ws2812.c
    )
endif()

avr_add_executable(
    charlie
    charlie.c
)

avr_add_executable(
    geiger
    geiger.c
)

avr_add_executable(
    alarm
    alarm.c
)

# 1MHz clock - -U lfuse:w:0x62:m -U hfuse:w:0xdf:m
avr_add_executable(
    candle
    candle.c
)

avr_add_executable(
    proximity
    proximity.c
)

if(${AVR_MCU} STREQUAL "attiny85")
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

    avr_add_executable(
        ds3231_oled_clock
        i2c_master_usi.c
        u8x8_avr.c
        ds3231_oled_clock.c
    )

    avr_target_link_libraries(ds3231_oled_clock u8g2)

    avr_add_executable(
        nano_ammeter
        i2c_master_usi.c
        u8x8_avr.c
        nano_ammeter.c
    )

    avr_target_link_libraries(nano_ammeter u8g2)
endif()

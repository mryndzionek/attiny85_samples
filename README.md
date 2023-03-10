attiny85 sample apps and libraries
==================================

Introduction
------------

Attiny85 MCU - some tests and example apps + I2C and LCD libraries


Configuring
-----------

```
cmake -DAVR_PROGRAMMER=avrisp -DAVR_PORT="/dev/ttyACM0" \
      -DCMAKE_TOOLCHAIN_FILE=../toolchain-avr-gcc.make ..
```


Contact
-------
If you have questions, contact Mariusz Ryndzionek at:

<mryndzionek@gmail.com>

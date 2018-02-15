# Tiny13 Firefles #

- the LEDs flicker randomly immitating fireflies
- 6 LEDs on 3 pins(PB0-PB2) using charlieplexing and bit angle modulation(more efficient PWM alternative for leds) for dimming
- BAM implemented with hardware timer in CTC mode
- could easily be scaled to 20 LEDs with almost no code changes(rewriting charlieplexing bitmasks)
- randomness implemented with linear congruential generator with seed persisted in eeprom and incremented on each power on
- 0.25 mA power consumption on a 3V CR2032

## TODO

- watchdog for even deeper sleep duting dim phase
- button for toggling power(manual deep sleep)
- reduce parasitic flickering on powered down LEDs
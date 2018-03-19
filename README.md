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

### Example videos
[![demo board](https://i.imgur.com/Mfi7AtL.png?2)](https://i.imgur.com/cufzkq7.mp4) [![bottled](https://i.imgur.com/VsxKpeM.png)](https://i.imgur.com/o5OHXny.mp4)

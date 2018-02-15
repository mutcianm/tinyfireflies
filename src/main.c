#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>

#define ROR(x) (((x) >> 1) | ((x) << 7))
#define LED_ON(x)  do {DDRB = chrlp[(x)] | 0xF0; PORTB = chrlp[(x)] >> 4;} while(0)
#define LED_OFF()  do {DDRB = 0xff; PORTB = 0x00;} while(0)

#define SLOWNESS 18                     // how infrequently we should try changing brightness
#define ACCEL 56                        // chance of actually changing the brightness (higher - less likely)
#define DIM_L 57                        // chance of staying dim (ACCEL+1 is highest)

register uint8_t occount asm("r3");
register uint8_t led_i   asm("r4");
register uint8_t s       asm("r5"); // lcg var
register uint8_t a       asm("r6"); // lcg var

// charlieplexing configurations of B: 4 high bits - PORTB; 4 low bits - DDRB
uint8_t chrlp[] = {0b00011011, 0b00101011, 0b00101110, 0b01001110, 0b00011101, 0b01001101};

// hand picked gamma corrected brightness levels
uint8_t ramp[] = {0,1,5,7,15,23,50,73,112,254,112,73,51,23,15,7,5,1,0};

// per-led indexes in ramp array
uint8_t brightness[] = {0,0,0,0,0,0};


static uint8_t rand(void) {
    s ^= s << 3;
    s ^= s >> 5;
    s ^= a++ >> 2;
    return s;
}

ISR(TIM0_COMPA_vect) {
    TCNT0 = 0;
    OCR0A = ROR(OCR0A);                     // decrease next timer delay by ^2, shift bit index in brightness level
    if (OCR0A & ramp[brightness[led_i]]) {  // OCR0A cycles through brightness level bits
        LED_ON(led_i);                      // if current bit is set, turn on the led
    } else {
        LED_OFF();                          // or off if not
    }
    if (OCR0A == 0b10000000) {                      // cycled through all current led's brightness bits
        if (led_i < 5) ++led_i; else led_i = 0;     // select next led
        if (++occount > SLOWNESS) {                 // how often we should change led brightness
            for (uint8_t i = 0; i < sizeof(brightness); ++i) {
                uint8_t r = rand(); // NOLINT
                if (r > ACCEL) {                                        // chance advancing brightness
                    if ((r > DIM_L)  && (brightness[i] == 0)) continue; // chance of staying in off state
                    if (brightness[i] < sizeof(ramp)-1) ++brightness[i]; else brightness[i] = 0;
                }
            }
            occount = 0;
        }
    }
}

int main(void) {
    occount = 0;
    led_i = 0;

    s = eeprom_read_byte((uint8_t*)16);     // read lcg seed from eeprom on startup
    eeprom_busy_wait();
    eeprom_write_byte((uint8_t*)16, ++s);   // save updated seed for next boot
    eeprom_busy_wait();

    TCCR0B = (1 << CS01);   // timer prescaler == 8
    TCCR0A = (1 << WGM01);  // timer set to CTC mode
    TIMSK0 = (1 << OCIE0A); // enable CTC timer interrupt
    OCR0A = 0b10000000;     // this bit will be shifted right on every interrupt

    sei();

    set_sleep_mode(SLEEP_MODE_IDLE);    // only disable cpu and flash clk while sleeping
    while(1) { sleep_mode(); }          // sleep until timer interrupt is triggered
}


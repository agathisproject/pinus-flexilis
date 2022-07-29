// SPDX-License-Identifier: GPL-3.0-or-later

#include "gpio.h"

#if defined(__AVR__)
#include <avr/io.h>
#elif defined(__XC16__)
#include "pin_manager.h"
#endif

#if defined(__linux__)
void gpio_cfg(PIN_ID_t pinId, PIN_TYPE_t type) {

}
#elif defined(__AVR__)
void gpio_cfg(PIN_ID_t pinId, PIN_TYPE_t type) {
    switch (pinId) {
        case PIN_2:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD2);
            } else {
                DDRD &= ~_BV(DDD2);
            }
            break;
        case PIN_3:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD3);
            } else {
                DDRD &= ~_BV(DDD3);
            }
            break;
        case PIN_4:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD4);
            } else {
                DDRD &= ~_BV(DDD4);
            }
            break;
        case PIN_5:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD5);
            } else {
                DDRD &= ~_BV(DDD5);
            }
            break;
        case PIN_6:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD6);
            } else {
                DDRD &= ~_BV(DDD6);
            }
            break;
        case PIN_7:
            if (type == PIN_TYPE_OUT) {
                DDRD |= _BV(DDD7);
            } else {
                DDRD &= ~_BV(DDD7);
            }
            break;
        case PIN_8:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB0);
            } else {
                DDRB &= ~_BV(DDB0);
            }
            break;
        case PIN_9:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB1);
            } else {
                DDRB &= ~_BV(DDB1);
            }
            break;
        case PIN_10:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB2);
            } else {
                DDRB &= ~_BV(DDB2);
            }
            break;
        case PIN_11:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB3);
            } else {
                DDRB &= ~_BV(DDB3);
            }
            break;
        case PIN_12:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB4);
            } else {
                DDRB &= ~_BV(DDB4);
            }
            break;
        case PIN_13:
            if (type == PIN_TYPE_OUT) {
                DDRB |= _BV(DDB5);
            } else {
                DDRB &= ~_BV(DDB5);
            }
            break;
        default:
            break;
    }
}
#elif defined(__XC16__)
void gpio_cfg(PIN_ID_t pinId, PIN_TYPE_t type) {

}
#endif

#if defined(__linux__)
void gpio_set(PIN_ID_t pinId, uint8_t val) {

}
#elif defined(__AVR__)
void gpio_set(PIN_ID_t pinId, uint8_t val) {
    switch (pinId) {
        case PIN_2:
            if (val == 0) {
                PORTD &= ~_BV(PORTD2);
            } else {
                PORTD |= _BV(PORTD2);
            }
            break;
        case PIN_3:
            if (val == 0) {
                PORTD &= ~_BV(PORTD3);
            } else {
                PORTD |= _BV(PORTD3);
            }
            break;
        case PIN_4:
            if (val == 0) {
                PORTD &= ~_BV(PORTD4);
            } else {
                PORTD |= _BV(PORTD4);
            }
            break;
        case PIN_5:
            if (val == 0) {
                PORTD &= ~_BV(PORTD5);
            } else {
                PORTD |= _BV(PORTD5);
            }
            break;
        case PIN_6:
            if (val == 0) {
                PORTD &= ~_BV(PORTD6);
            } else {
                PORTD |= _BV(PORTD6);
            }
            break;
        case PIN_7:
            if (val == 0) {
                PORTD &= ~_BV(PORTD7);
            } else {
                PORTD |= _BV(PORTD7);
            }
            break;
        case PIN_8:
            if (val == 0) {
                PORTB &= ~_BV(PORTB0);
            } else {
                PORTB |= _BV(PORTB0);
            }
            break;
        case PIN_9:
            if (val == 0) {
                PORTB &= ~_BV(PORTB1);
            } else {
                PORTB |= _BV(PORTB1);
            }
            break;
        case PIN_10:
            if (val == 0) {
                PORTB &= ~_BV(PORTB2);
            } else {
                PORTB |= _BV(PORTB2);
            }
            break;
        case PIN_11:
            if (val == 0) {
                PORTB &= ~_BV(PORTB3);
            } else {
                PORTB |= _BV(PORTB3);
            }
            break;
        case PIN_12:
            if (val == 0) {
                PORTB &= ~_BV(PORTB4);
            } else {
                PORTB |= _BV(PORTB4);
            }
            break;
        case PIN_13:
            if (val == 0) {
                PORTB &= ~_BV(PORTB5);
            } else {
                PORTB |= _BV(PORTB5);
            }
            break;
        default:
            break;
    }
}
#elif defined(__XC16__)
void gpio_set(PIN_ID_t pinId, uint8_t val) {
    switch (pinId) {
        case PIN_RA0:
            if (val == 0) {
                IO_RA0_SetLow();
            } else {
                IO_RA0_SetHigh();
            }
            break;
        case PIN_RA1:
            if (val == 0) {
                IO_RA1_SetLow();
            } else {
                IO_RA1_SetHigh();
            }
            break;
        case PIN_RC7:
            if (val == 0) {
                IO_RC7_SetLow();
            } else {
                IO_RC7_SetHigh();
            }
            break;
        default:
            break;
    }
}
#endif

#if defined(__linux__)
uint8_t gpio_get(PIN_ID_t pinId) {
    return 0;
}
#elif defined(__AVR__)
uint8_t gpio_get(PIN_ID_t pinId) {
    switch (pinId) {
        case PIN_2:
            return (PIND & _BV(PIND2)) >> PIND2;
        case PIN_3:
            return (PIND & _BV(PIND3)) >> PIND3;
        case PIN_4:
            return (PIND & _BV(PIND4)) >> PIND4;
        case PIN_5:
            return (PIND & _BV(PIND5)) >> PIND5;
        case PIN_6:
            return (PIND & _BV(PIND6)) >> PIND6;
        case PIN_7:
            return (PIND & _BV(PIND7)) >> PIND7;
        case PIN_8:
            return (PINB & _BV(PINB0)) >> PINB0;
        case PIN_9:
            return (PINB & _BV(PINB1)) >> PINB1;
        case PIN_10:
            return (PINB & _BV(PINB2)) >> PINB2;
        case PIN_11:
            return (PINB & _BV(PINB3)) >> PINB3;
        case PIN_12:
            return (PINB & _BV(PINB4)) >> PINB4;
        case PIN_13:
            return (PINB & _BV(PINB5)) >> PINB5;
        default:
            return 0;
    }
}
#elif defined(__XC16__)
uint8_t gpio_get(PIN_ID_t pinId) {
    switch (pinId) {
        case PIN_RC3:
            return IO_RC3_GetValue();
        case PIN_RC4:
            return IO_RC4_GetValue();
        case PIN_RC5:
            return IO_RC5_GetValue();
        default:
            return 0;
    }
}
#endif

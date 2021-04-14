#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

extern uint8_t pressed_keys[];
extern uint8_t shift;
extern uint8_t control;
extern uint8_t capslock;
extern uint8_t alt;
extern uint8_t scroll_lock;
extern uint8_t num_lock;

#endif
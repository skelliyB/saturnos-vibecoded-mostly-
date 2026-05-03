#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

uint8_t get_scancode(void);   // ❌ REMOVE if not used
char scancode_to_ascii(uint8_t sc);
char keyboard_read_char(void);

#endif
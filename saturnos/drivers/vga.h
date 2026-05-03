#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define WHITE_TXT 0x07

void kprint(const char* str, uint8_t color);

#endif
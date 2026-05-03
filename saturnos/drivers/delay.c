#include <stdint.h>
#include "io.h"

void pit_wait(uint16_t count) {
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp & ~0x03);

    outb(0x43, 0xB0);

    outb(0x42, count & 0xFF);
    outb(0x42, count >> 8);

    while (!(inb(0x61) & 0x20));
}
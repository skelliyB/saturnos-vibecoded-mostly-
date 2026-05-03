#include <stdint.h>
#include "io.h"

void mouse_poll(void){
    uint8_t status = inb(0x64);

    if (status & 1) {  // data available
        uint8_t data = inb(0x60);

        if (status & (1 << 5)) {
        }
    }
}

void mouse_init() {
    // enable auxiliary device (mouse)
    outb(0xA8, 0x64);

    // enable interrupts
    outb(0x20, 0x64);
    uint8_t status = inb(0x60) | 2;
    outb(0x60, 0x64);
    outb(status, 0x60);

    // tell mouse to use default settings
    outb(0xD4, 0x64);
    outb(0xF6, 0x60);
    inb(0x60); // ACK

    // enable streaming
    outb(0xD4, 0x64);
    outb(0xF4, 0x60);
    inb(0x60); // ACK
}
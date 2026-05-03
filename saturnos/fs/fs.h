#ifndef IF_H
#define IF_H
#include "stdint.h"
void ata_write_sector(uint32_t lba, uint16_t* buffer);
void ata_read_sector(uint32_t lba, uint16_t* buffer);


#endif
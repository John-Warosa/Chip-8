#ifndef ROMLOAD_H
#define ROMLOAD_H

#include "types.h"
#include <stddef.h>

void load_rom(u8 ram[], size_t start, const char *filename);

#endif // ROMLOAD_H

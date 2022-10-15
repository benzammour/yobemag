#ifndef YOBEMAG_LCD_H
#define YOBEMAG_LCD_H

#include <stdint.h>
#include <SDL2/SDL.h>

#include "errcodes.h"

ErrorCode lcd_load(void);
ErrorCode lcd_step(void);

#endif //YOBEMAG_LCD_H

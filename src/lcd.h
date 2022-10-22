#ifndef YOBEMAG_LCD_H
#define YOBEMAG_LCD_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

void lcd_init(void);
void lcd_teardown(void);
bool lcd_step(void);

#endif // YOBEMAG_LCD_H

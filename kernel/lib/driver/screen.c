#include <driver/screen.h>

extern char font[];
extern unsigned char font_height;
extern unsigned char font_width;

void kputc(char c, uint16_t xp, uint16_t yp){
    for(int byte = 0; byte < font_width * font_height / 8; byte++){
        uint8_t curr_byte = font[c * 16 + byte];
        for(int bit = 0; bit < 8; bit++){
            if(!!(curr_byte & (1 << bit)) == 1)drawPixel(bit + xp, byte + yp);
        }
    }
}
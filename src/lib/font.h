// font.h
#ifndef FONT_H
#define FONT_H

enum {
    FONT_WIDTH     = 8,
    FONT_HEIGHT    = 8,
    FONT_BPG       = 8,  // Bytes per glyph
    FONT_BPL       = 1,  // Bytes per line
    FONT_NUMGLYPHS = 224
};

extern unsigned int vgapal[];
extern unsigned char font[FONT_NUMGLYPHS][FONT_BPG];

#endif // FONT_H

#include "applyCompOrDecomp.h"

void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
    A2Methods_Object* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct Pnm_rgb* decomped = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array,col, row);
    unsigned denominator = mycl->denom;
    decomped->red = (unsigned)(original->red * denominator);
    decomped->green = (unsigned)(original->green * denominator);
    decomped->blue = (unsigned)(original->blue * denominator);
}

void applyDecompToRGBFloat(int col, int row, A2 toBeFilled, 
    A2Methods_Object* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct rgbFloat *toBeSet = ptr;
    struct YPP* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->red = 1.0 * original->y + 0.0 * original->pb + 1.402 *
        original->pr;
    toBeSet->green = 1.0 * original->y - 0.344136 * original->pb - 
        0.714136 * original->pr;
    toBeSet->blue = 1.0 * original->y + 1.772 * original->pb + 0.0 * 
        original->pr;
}

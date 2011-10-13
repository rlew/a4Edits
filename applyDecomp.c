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

void decompToRGBInt(A2 intArray, struct Closure* cl){
    cl->methods->map_default(intArray, applyDecompToRGBInt, cl);
}


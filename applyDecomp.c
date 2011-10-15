#include "applyCompOrDecomp.h"
#include "arith40.h"
#include "assert.h"

/* Decompression: void* ptr is a struct of Pnm_rgb to be filled with the
 * calculations performed on the floatRGB array in the closure. */
void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
                                                void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct Pnm_rgb* decomped = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array,col, row);
    unsigned int  denominator = mycl->denom;
    //assert(original->red <= 1.0);
    //assert(original->green<= 1.0);
    //assert(original->blue <= 1.0);
    decomped->red = original->red * denominator;
    decomped->green = original->green * denominator;
    decomped->blue = original->blue * denominator;
}

/* Decompression: void* ptr is a struct of rgbFloat to be filled with the
 * calculations performed on the YPP array in the closrure. */
void applyDecompToRGBFloat(int col, int row, A2 toBeFilled,
                                              void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct rgbFloat *toBeSet = ptr;
    struct YPP* original = mycl->methods->at(mycl->array, col, row);
    //assert(original->y <= 1.0);
    //assert(original->pb<= 0.5);
    //assert(original->pr <= 0.5);
    toBeSet->red = 1.0 * original->y + 0.0 * original->pb
                                                  + 1.402 * original->pr;
    toBeSet->green = 1.0 * original->y - 0.344136 * original->pb
                                                - 0.714136 * original->pr;
    toBeSet->blue = 1.0 * original->y + 1.772 * original->pb
                                                    + 0.0 * original->pr;
}

/* Decompression: void *ptr is a struct of YPP to be filled with the
 * calculations performed on the AvgDCT array in the closure. */
void applyDecompToYPP(int col, int row, A2 toBeFilled,
                                                    void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct YPP* toBeSet = ptr;
    struct AvgDCT* original = mycl->methods->at(mycl->array, col/2, row/2);

    if(col%2 == 0 && row%2 == 0)
        toBeSet->y = original->a - original->b - original->c + original->d;
    else if (col%2 == 1 && row%2 == 0)
        toBeSet->y = original->a - original->b + original->c - original->d;

    else if (col%2 == 0 && row%2 == 1)
        toBeSet->y = original->a + original->b - original->c - original->d;
    else
        toBeSet->y = original->a + original->b + original->c + original->d;

    toBeSet->pb = original->pb;
    toBeSet->pr = original->pr;
}

static float convertToFloat(int num) {
    num = num + 15;
    //float a[31] = {-0.3, -0.28, -0.26, -0.24, -0.22, -0.2, -0.18, -0.16,
    //-0.14, -0.12, -0.1, -0.08, -0.06, -0.04, -0.02, 0.0, 0.02, 0.04, 0.06,
    //0.08, 0.1, 0.12, 0.14, 0.16, 0.18, 0.20, 0.22, 0.24, 0.26, 0.28, 0.3};
    float a[31] = {-0.3, -0.28, -0.26, -0.24, -0.22, -0.2, -0.18, -0.16,
    -0.14, -0.12, -0.1, -0.077, -0.055, -0.033, -0.011, 0.0, 0.011, 0.033,
    0.055, 0.077, 0.1, 0.12, 0.14, 0.16, 0.18, 0.20, 0.22, 0.24, 0.26, 0.28, 
    0.3};
    return a[(int)num]; 
}

void applyDecompToAvgDCT(int col, int row, A2 toBeFilled,
    void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct AvgDCT* toBeSet = ptr;
    struct AvgDCTScaled* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->pb = Arith40_chroma_of_index(original->pb);
    toBeSet->pr = Arith40_chroma_of_index(original->pr);
    toBeSet->a = (float)(original->a)/511.0;
    toBeSet->b = convertToFloat(original->b);
    toBeSet->c = convertToFloat(original->c);
    toBeSet->d = convertToFloat(original->d);
}

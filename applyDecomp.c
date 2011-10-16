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

/* Decompression: void* ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the AvgDCTScaled array in the closure */
void applyDecompToAvgDCT(int col, int row, A2 toBeFilled,
    void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure *mycl = cl;
    struct AvgDCT* toBeSet = ptr;
    struct AvgDCTScaled* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->pb = Arith40_chroma_of_index(original->pb);
    toBeSet->pr = Arith40_chroma_of_index(original->pr);
    toBeSet->a = ((float)(original->a))/511.0;
    toBeSet->b = ((float)(original->b))/50.0;
    toBeSet->c = ((float)(original->c))/50.0;
    toBeSet->d = ((float)(original->d))/50.0;
    //if(toBeSet->b > .300000) 
      //printf("ToBeSet b is: %f, row: %d, col %d\n", toBeSet->b, row, col);
    //assert(toBeSet->b <= .300000 );
      //  assert( toBeSet->b >= -.300000);
    //assert(toBeSet->c <= .300000 || toBeSet->c >= -.300000);
    //assert(toBeSet->d >= -.300000 || toBeSet->d <= .300000);
}


#include "applyCompOrDecomp.h"
#include <math.h>
#include "arith40.h"

/* Compression: trimming the image dimesnions to make them even. */
void compTrimPixmap(Pnm_ppm image) {
    if(image->width % 2 != 0) {
      image->width -= 1;
    }
    if(image->height % 2 != 0) {
      image->height -= 1;
    }
}
/* Compression: void *ptr is a struct of rgbFloat's to be filled with the
 * calculations preformed on the Pnm_rgb array in the closure. */
void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                void* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct rgbFloat* toBeSet = ptr;
    struct Pnm_rgb* original = mycl->methods->at(mycl->array, col, row);
    float denom = (float)mycl->denom;
    toBeSet->red = (float)(original->red) / denom;
    toBeSet->green = (float)(original->green) / denom;
    toBeSet->blue = (float)(original->blue) / denom;
}

/* Compression: void *ptr is a struct of YPP to be filled with the calculations
 * preformed on the rgbFloat array in the closure.*/
void applyCompToYPP(int col, int row, A2 toBeFilled, void* ptr,
                                                          void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct YPP* toBeSet = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->y = 0.299 * original->red + 0.587 * original->green
                                                    + 0.114 * original->blue;
    toBeSet->pb = -0.168736 * original->red - 0.331264 * original->green
                                                      + 0.5 * original->blue;
    toBeSet->pr = 0.5 * original->red - 0.418688 * original->green
                                                  - 0.081312 * original->blue;
}

/* Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations performed on the YPP array in the closure */
void applyCompToAvgDCT(int col, int row, A2 toBeFilled, void* ptr,
    void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct AvgDCT* toBeSet = ptr;
    struct YPP* original1 = mycl->methods->at(mycl->array, col*2, row*2);
    struct YPP* original2 = mycl->methods->at(mycl->array, col*2 + 1, row*2);
    struct YPP* original3 = mycl->methods->at(mycl->array, col*2, row*2+1);
    struct YPP* original4 = mycl->methods->at(mycl->array, col*2 + 1,
        row*2 + 1);
    toBeSet->pb = (original1->pb + original2->pb + original3->pb +
        original4->pb) / 4.0;
    toBeSet->pr = (original1->pr + original2->pr + original3->pr +
        original4->pr) / 4.0;
    toBeSet->a = (original1->y + original2->y + original3->y +
        original4->y) / 4.0;
    toBeSet->b = (-original1->y - original2->y + original3->y +
        original4->y) / 4.0;
    toBeSet->c = (-original1->y + original2->y - original3->y +
        original4->y) / 4.0;
    toBeSet->d = (original1->y - original2->y - original3->y +
        original4->y) / 4.0;
}

/* Compression: converts the arguments of floats and returns a scaled integer in
 * the range of plus or minus 15. */
static int convertToScaledInt(float num) {
    int returnIndex = 0;
    //float a[31] = {-0.3, -0.28, -0.26, -0.24, -0.22, -0.2, -0.18, -0.16,
    //-0.14, -0.12, -0.1, -0.08, -0.06, -0.04, -0.02, 0.0, 0.02, 0.04, 0.06,
    float a[31] = {-0.3, -0.28, -0.26, -0.24, -0.22, -0.2, -0.18, -0.16,
    -0.14, -0.12, -0.1, -0.077, -0.055, -0.033, -0.011, 0.0, 0.011, 0.033,
    0.055, 0.077, 0.1, 0.12, 0.14, 0.16, 0.18, 0.20, 0.22, 0.24, 0.26, 0.28, 
    0.3};
//    float a[16] = {-0.35, -0.20, -0.15, -0.1, -0.077, -0.055, -0.033, -0.011,
//    float a[16] = {-0.35, -0.20, -0.15, -0.1, -0.077, -0.055, -0.033, -0.011,
//    0.011, 0.033, 0.055, 0.077, 0.1, 0.15, 0.2, 0.35};
    for (int i = 0; i < 31; i++) {
        if(fabs(num - a[i]) < fabs(num - a[returnIndex])) {
            returnIndex = i;
        }
    }
    return returnIndex - 15;
}

/* Compression: void *ptr is a struct of AvgDCT to be filled with the
 * calculations preformed on the AvgDCTScalled array in the closure. */
void applyCompToAvgDCTScaled(int col, int row, A2 toBeFilled, void* ptr,
    void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct AvgDCTScaled* toBeSet = ptr;
    struct AvgDCT* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->pb = Arith40_index_of_chroma(original->pb);
    toBeSet->pr = Arith40_index_of_chroma(original->pr);
    toBeSet->a = (unsigned)(original->a * 511);
    toBeSet->b = convertToScaledInt(original->b);
    toBeSet->c = convertToScaledInt(original->c);
    toBeSet->d = convertToScaledInt(original->d);
}

#include "applyCompOrDecomp.h"

/* Compression: trimming the image dimesnions to make them even */
void compTrimPixmap(Pnm_ppm image) {
    if(image->width % 2 != 0) {
      image->width -= 1;
    }
    if(image->height % 2 != 0) {
      image->height -= 1;
    }
}
/* Compression: takes the integer array and float array */
void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                A2Methods_Object* ptr, void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct rgbFloat* toBeSet = ptr;
    struct Pnm_rgb* original = mycl->methods->at(mycl->array, col, row);
    float denom = (float)mycl->denom;
    toBeSet->red = (float)(original->red) / denom;
    toBeSet->green = (float)(original->green) / denom;
    toBeSet->blue = (float)(original->blue) / denom;
}

void applyCompToYPP(int col, int row, A2 toBeFilled, A2Methods_Object* ptr,
    void* cl) {
    (void) toBeFilled;
    struct Closure* mycl = cl;
    struct YPP* toBeSet = ptr;
    struct rgbFloat* original = mycl->methods->at(mycl->array, col, row);
    toBeSet->y = 0.299 * original->red + 0.587 * original->green + 0.114 *
        original->blue;
    toBeSet->pb = -0.168736 * original->red - 0.331264 * original->green +
        0.5 * original->blue;
    toBeSet->pr = 0.5 * original->red - 0.418688 * original->green - 
        0.081312 * original->blue;
}

void applyCompToAvgDCT(int col, int row, A2 toBeFilled, A2Methods_Object* ptr,
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

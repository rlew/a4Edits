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

void compToRGBFloat(A2 floatArray, struct Closure* cl) {
    //map(floatArray, applyCompToRGBFloat, cl);
    cl->methods->map_default(floatArray, applyCompToRGBFloat, cl);
}


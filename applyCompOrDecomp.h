#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct rgbFloat {
    float red, green, blue;
}rgbFloat;

typedef struct YPP {
    float y, pb, pr;
}YPP;

typedef struct Closure {
    A2Methods_T methods;
    A2 array;
    unsigned denom;
}Closure;

/* Compression functions */
extern void compTrimPixmap(Pnm_ppm image);
extern void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                A2Methods_Object* ptr, void* cl);
extern void compToRGBFloat(A2 floatArray, struct Closure* cl);

/* Decompression functions */
extern void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
    A2Methods_Object* ptr, void* cl);
extern void decompToRGBInt(A2 intArray, struct Closure* cl);

#include "pnm.h"
#include "a2methods.h"

#define A2 A2Methods_UArray2

typedef struct rgbFloat {
    float red, green, blue;
}rgbFloat;

typedef struct YPP {
    float y, pb, pr;
}YPP;

typedef struct AvgDCT {
    float pb, pr, a, b, c, d;
}AvgDCT;

typedef struct AvgDCTScaled {
    unsigned pb, pr, a;
    int b, c, d;
}AvgDCTScaled;

typedef struct Closure {
    A2Methods_T methods;
    A2 array;
    unsigned denom;
}Closure;

/* Compression functions */
extern void compTrimPixmap(Pnm_ppm image);
extern void applyCompToRGBFloat(int col, int row, A2 toBeFilled,
                                void* ptr, void* cl);
extern void applyCompToYPP(int col, int row, A2 toBeFilled, void*
    ptr, void* cl);
extern void applyCompToAvgDCT(int col, int row, A2 toBeFilled, 
                              void* ptr, void* cl);
extern void applyCompToAvgDCTScaled(int col, int row, A2 toBeFilled,
                                 void* ptr, void* cl);

/* Decompression functions */
extern void applyDecompToRGBInt(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);
extern void applyDecompToRGBFloat(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);
extern void applyDecompToYPP(int col, int row, A2 toBeFilled,
    void* ptr, void* cl);
extern void applyDecompToAvgDCT(int col, int row,
    A2 toBeFilled, void* ptr, void* cl);

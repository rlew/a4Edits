#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "stdlib.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "applyCompOrDecomp.h"


/* this fn only works with FLOATS */
static void applyCompPrint(int col, int row, A2 array, A2Methods_Object* ptr,
    void* cl) {
    (void) col; (void) row; (void) array; (void) cl;
    struct rgbFloat* elem = ptr;
    fprintf(stdout, "%f %f %f ", elem->red, elem->green, elem->blue);
}

/* writes the array usray to std output using maping fn */
static void compWrite(A2 array, A2Methods_T methods) {
    unsigned height = (unsigned)methods->height(array); 
    unsigned width = (unsigned)methods->width(array);
    fprintf(stdout, "COMP40 Compressed image format 2\n%u %u\n", width,
        height);
    methods->map_default(array, applyCompPrint, NULL);
}

void compress40(FILE *input) { 
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_mapfun *map = methods->map_default;
    assert(map);
    #define SET_METHODS(METHODS,MAP,WHAT) do {\
        methods = (METHODS); \
        assert(methods); \
        map = methods->MAP; \
        if(!map) { \
            frprintf(stderr, "%s does not support " WHAT " mapping\n", \
            argv[0]); \
            exit(1); \
        } \
    } while(0)
    Pnm_ppm image;
    TRY
        image = Pnm_ppmread(input, methods);
    EXCEPT(Pnm_Badformat)
        fprintf(stderr, "Badly formatted file.\n");
        exit(1);
    END_TRY;

    compTrimPixmap(image);

    // convert RGB Ints to RGB Floats
    A2 floatArray = methods->new(image->width, image->height,
                                 sizeof(struct rgbFloat));
    struct Closure cl = { methods, image->pixels, image->denominator };
    compToRGBFloat(floatArray, &cl);

    compWrite(floatArray, methods);
    methods->free(&floatArray);
    Pnm_ppmfree(&image);
}



/*---------------------------------------------------------------------------*/
/* filing a float array from std input */
static void fillFloatArray(int col, int row, A2 array, A2Methods_Object* ptr,
    void* cl) {
    (void) col; (void) row; (void) array;
    FILE* input = cl;
    struct rgbFloat* curpix = ptr;
    struct rgbFloat elem;
    fscanf(input, "%f", &elem.red);
    fscanf(input, "%f", &elem.green); 
    fscanf(input, "%f", &elem.blue);
    *curpix = elem;
}
/* testing wrapper fn */
static void decompRead(FILE* input, A2 floatArray, A2Methods_T methods) {
    methods->map_default(floatArray, fillFloatArray, input);
}

void decompress40(FILE *input) { 
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    A2Methods_mapfun *map = methods->map_default;
    assert(map);
    #define SET_METHODS(METHODS,MAP,WHAT) do {\
        methods = (METHODS); \
        assert(methods); \
        map = methods->MAP; \
        if(!map) { \
            frprintf(stderr, "%s does not support " WHAT " mapping\n", \
            argv[0]); \
            exit(1); \
        } \
    } while(0)
    unsigned height, width;
    int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width,
        &height);
    assert(read == 2);
    int c = getc(input);
    assert(c == '\n');

    A2 floatArray = methods->new(width, height, sizeof(struct rgbFloat));
    decompRead(input, floatArray, methods);

    A2 intArray = methods->new(width, height, sizeof(struct Pnm_rgb));
    struct Closure cl = { methods, floatArray, 255 };
    decompToRGBInt(intArray, &cl);
    
    Pnm_ppm output;
    NEW(output);
    output->denominator = 255;
    output->width = width;
    output->height = height;
    output->pixels = intArray;
    output->methods = methods;

    Pnm_ppmwrite(stdout, output);
    
    methods->free(&floatArray);
    Pnm_ppmfree(&output);
}

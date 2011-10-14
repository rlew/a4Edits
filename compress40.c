#include "compress40.h"
#include "pnm.h"
#include "assert.h"
#include "stdlib.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "mem.h"
#include "applyCompOrDecomp.h"


/* this fn only works with AVG and DCT structs */
static void applyCompPrint(int col, int row, A2 array, A2Methods_Object* ptr,
    void* cl) {
    (void) col; (void) row; (void) array; (void) cl;
    struct AvgDCT* elem = ptr;
    fprintf(stdout, "%f %f %f %f %f %f\n", elem->pb, elem->pr, elem->a,
                                       elem->b, elem->c, elem->d);
}

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
    methods->map_default(floatArray, applyCompToRGBFloat, &cl);

    A2 yppArray = methods->new(methods->width(floatArray),
                               methods->height(floatArray),
                               sizeof(struct YPP));
    cl.array = floatArray;
    methods->map_default(yppArray, applyCompToYPP, &cl);

    A2 avgDCTArray = methods->new((methods->width(yppArray))/2, 
                                  (methods->height(yppArray))/2,
                                  sizeof(struct AvgDCT));
    cl.array = yppArray;
    methods->map_default(avgDCTArray, applyCompToAvgDCT, &cl);

    compWrite(avgDCTArray, methods);
    methods->free(&avgDCTArray);
    methods->free(&yppArray);
    methods->free(&floatArray);
    Pnm_ppmfree(&image);
}



/*---------------------------------------------------------------------------*/

/* this function only works with Avg and DCT struct */
static void fillToReadArray(int col, int row, A2 array, A2Methods_Object* ptr,
    void* cl) {
    (void) col; (void) row; (void) array;
    FILE* input = cl;
    struct AvgDCT* curpix = ptr;
    struct AvgDCT elem;
    fscanf(input, "%f", &elem.pb); 
    fscanf(input, "%f", &elem.pr);
    fscanf(input, "%f", &elem.a);
    fscanf(input, "%f", &elem.b);
    fscanf(input, "%f", &elem.c);
    fscanf(input, "%f", &elem.d);
    *curpix = elem;
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

    A2 avgDCTArray = methods->new(width, height, sizeof(struct AvgDCT));
    methods->map_default(avgDCTArray, fillToReadArray, input);

    A2 yppArray = methods->new(width*2, height*2, sizeof(struct YPP));
    struct Closure cl = { methods, avgDCTArray, 255 };
    methods->map_default(yppArray, applyDecompToYPP, &cl);

    A2 floatArray = methods->new(width*2, height*2, sizeof(struct rgbFloat));
    cl.array = yppArray;
    methods->map_default(floatArray, applyDecompToRGBFloat, &cl);

    A2 intArray = methods->new(width*2, height*2, sizeof(struct Pnm_rgb));
    cl.array = floatArray;
    methods->map_default(intArray, applyDecompToRGBInt, &cl);
    
    Pnm_ppm output;
    NEW(output);
    output->denominator = 255;
    output->width = width*2;
    output->height = height*2;
    output->pixels = intArray;
    output->methods = methods;

    Pnm_ppmwrite(stdout, output);
    
    methods->free(&floatArray);
    methods->free(&yppArray);
    methods->free(&avgDCTArray);
    Pnm_ppmfree(&output);
}

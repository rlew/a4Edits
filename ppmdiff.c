#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "uarray2.h"
#include "pnm.h"
#include "mem.h"
#include "math.h"

struct RGB {
   unsigned red;
   unsigned green;
   unsigned blue;
};


int main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("Not enough arguments to compare\n");
    exit(1);
  }
    Pnm_ppm image1;
    Pnm_ppm image2;
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);
    assert(argc == 3);
    FILE* fp = fopen(argv[1], "r");
    assert(fp);
    TRY
        image1 = Pnm_ppmread(fp, methods);
    EXCEPT(Pnm_Badformat)
        fprintf(stderr, "Badly formatted file.\n");
        exit(1);
    END_TRY;
    if(fp != stdin) {
        fflush(fp);
    }
    fp = fopen(argv[2], "r");
    assert(fp);
    TRY
        image2 = Pnm_ppmread(fp, methods);
    EXCEPT(Pnm_Badformat)
        fprintf(stderr, "Badly formatted file.\n");
        exit(1);
    END_TRY;
    if(fp != stdin) {
        fclose(fp);
    }
    
    int width, height;
    double sum = 0;

    if(abs(image1->height - image2->height) <= 1) {
        if (image1->height > image2->height) {
            height = image2->height;
        }
        else {
            height = image1->height;
        }
    }
    else {
        fprintf(stderr, "Height doesn't match.\n");
        exit(1);
    }
    if(abs(image1->width - image2->width) <= 1) {
        if (image1->width > image2->width) {
            width = image2->width;
        }
        else {
            width = image1->width;
        }
    }
    else {
        fprintf(stderr, "Height doesn't match.\n");
        exit(1);
    }
    struct Pnm_rgb *temp1;
    struct Pnm_rgb *temp2;
    NEW(temp1);
    NEW(temp2);
    //should loop through every element and square the difference
    //between each rgb element
    for(int r = 0; r < height; r++) {
        for(int c = 0; c < width; c++) {
            temp1 = methods->at(image1->pixels, c, r);
            temp2 = methods->at(image2->pixels, c, r);
                 sum += (pow(((((double)(temp1->red))/(double)image1->denominator) - ((double)(temp2->red))/(double)image2->denominator), 2)
                     + pow(((((double)(temp1->green))/(double)image1->denominator) - ((double)(temp2->green))/(double)image2->denominator), 2)

                     + pow(((((double)(temp1->blue))/(double)image1->denominator) - ((double)(temp2->blue))/(double)image2->denominator), 2));


        }
    }
    /* Divide*/
    double E = sqrt(sum/(3.0*(double)height * (double)width));
    //sqrt
    //return
    //
    printf("E is: %f\n", E);
    return 0;
}

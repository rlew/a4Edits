#include <stdlib.h>

#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2; // private abbreviation

static A2 new(int width, int height, int size) {
  return UArray2_new(height, width, size);
}

static A2Methods_UArray2 new_with_blocksize(int width, int height,
    int size, int blocksize) {
    (void) blocksize;
    return UArray2_new(height, width, size);
}

static void a2free(A2 *array2p) {
    UArray2_free(*array2p);
}

static int width     (A2 array2)  { return UArray2_width(array2); }
static int height    (A2 array2)  { return UArray2_height(array2); }
static int size      (A2 array2)  { return UArray2_size(array2); }
// returns 0 because not applicable to implementation
static int blocksize (A2 array2)  { (void) array2; return 0; }

static A2Methods_Object* at(A2 array2, int i, int j) {
    return UArray2_at(array2, j, i);
}

typedef void applyfun(int col, int row, void* elem, void* cl);

struct a2fun_closure {
    A2Methods_applyfun *apply;
    void* cl;
    A2Methods_UArray2 array2;
};

static void apply_a2fun(int col, int row, void* elem, void* vcl)
{
    struct a2fun_closure *fcl = vcl;
    fcl->apply(row, col, fcl->array2, elem, fcl->cl);
}

static void map_row_major(A2 array2, A2Methods_applyfun apply, void* cl) {
    struct a2fun_closure mycl = { apply, cl, array2 };
    UArray2_map_row_major(array2, apply_a2fun, &mycl);
}

static void map_col_major(A2 array2, A2Methods_applyfun apply, void* cl) {
    struct a2fun_closure mycl = { apply, cl, array2 };
    UArray2_map_col_major(array2, apply_a2fun, &mycl);
}

struct small_closure {
    A2Methods_smallapplyfun *apply;
    void* cl;
    A2Methods_UArray2 array2;
};

static void apply_small(int col, int row, void* elem, void* vcl) {
    struct small_closure *cl = vcl;
    (void) row;
    (void) col;
    cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2 array2, A2Methods_smallapplyfun 
    apply, void* cl) {
    struct small_closure mycl = { apply, cl, array2 };
    UArray2_map_row_major(array2, apply_small, &mycl);
}

static void small_map_col_major(A2 array2, A2Methods_smallapplyfun 
    apply, void* cl) {
    struct small_closure mycl = { apply, cl, array2 };
    UArray2_map_col_major(array2, apply_small, &mycl);
}

 

// now create the private struct containing pointers to the functions

static struct A2Methods_T uarray2_methods_plain_struct = {
  new,
  new_with_blocksize,
  a2free,
  width,
  height,
  size,
  blocksize,
  at,
  map_row_major,
  map_col_major,
  NULL, // map_block_major
  map_row_major, // map_default
  small_map_row_major,
  small_map_col_major,
  NULL, // small_map_block_major
  small_map_row_major, // small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;


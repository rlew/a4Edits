#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

extern int UArray2_height(T twoDArray);
extern int UArray2_width(T twoDArray);
extern int UArray2_size(T twoDArray);
extern T UArray2_new(int height, int width, int size);
extern void UArray2_free(T twoDArray);
extern void * UArray2_at(T twoDArray, int row, int col);
extern void UArray2_map_row_major(T twoDArray, void apply(int row, int col, 
    void* elem, void * cl), void * cl);
extern void UArray2_map_col_major(T twoDArray, void apply(int row, int col, 
    void* elem, void *cl), void * cl);

#undef T
#endif

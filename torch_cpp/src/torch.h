#include <stdbool.h>

#define EXPORT __attribute__((visibility("default"))) __attribute__((used))
#ifdef __cplusplus 
#define EXPORT extern "C" __attribute__((visibility("default"))) __attribute__((used))
#endif

EXPORT double init_Torch();
EXPORT double init_OnnxModel();
EXPORT char* do_Json();
EXPORT double do_MatrixOps(double x);

EXPORT void create_Stack();
EXPORT void push_Stack(int x);
EXPORT int pop_Stack();
EXPORT void delete_Stack();
EXPORT bool isEmpty_Stack();
EXPORT int len_Stack();
EXPORT int do_Stack(int a, int b);

EXPORT char* add_Str(char* x);
EXPORT int* add_Array1D(int size);
EXPORT int** add_Array2D(int rows, int columns);
EXPORT int count_Array1D(int* array, int rows);
EXPORT int count_Array2D(int** array, int rows, int columns);
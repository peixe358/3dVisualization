#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <time.h>


/* Error messages */

#define MSG1  "Cannot allocate memory space"
#define MSG2  "Cannot open file"

/* Common data types to all programs */ 

#ifndef __cplusplus
#ifndef _WIN32
#ifndef __cplusplus
typedef enum boolean {false,true} bool;
#endif
#else
typedef unsigned short ushort;
#endif
#endif
typedef struct timeval timer;
typedef unsigned char uchar;

typedef struct _point{
	float x, y, z;
} Vector, Point;

typedef struct _voxel {
    int x, y, z;
} Voxel;

/* Common definitions */


#define PI          3.1415926536
#define INTERIOR    0
#define EXTERIOR    1
#define BOTH        2
#define RED         0
#define GREEN       1
#define BLUE        2
#define WHITE       0 
#define GRAY        1
#define BLACK       2
#define NIL        -1
#define INCREASING  1
#define DECREASING  0
#define Epsilon     1E-05  
#define Y           0
#define CG          1
#define CO          2 
#define AXIS_X  0
#define AXIS_Y  1
#define AXIS_Z  2    

/* Common operations */

#ifndef MAX
#define MAX(x,y) (((x) > (y))?(x):(y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) < (y))?(x):(y))
#endif

#ifndef ValidVoxel
#define ValidVoxel(img, v)  ((v.x >= 0) && (v.x < img->nx) && (v.y >= 0) && (v.y < img->ny) && (v.z >= 0) && (v.z < img->nz))
#endif

#define ROUND(x) ((x < 0)?(int)(x-0.5):(int)(x+0.5))

#define SIGN(x) ((x >= 0)?1:-1)

int     AlmostZero(double x); /* Check if variable is almost zero*/
char   *AllocCharArray(int n);  /* It allocates 1D array of n characters */
uchar  *AllocUCharArray(int n);  /* It allocates 1D array of n characters */
ushort *AllocUShortArray(int n);  /* It allocates 1D array of n characters */
int    *AllocIntArray(int n);   /* It allocates 1D array of n integers */
float  *AllocFloatArray(int n); /* It allocates 1D array of n floats */
double *AllocDoubleArray(int n);/* It allocates 1D array of n doubles */

void Error(char *msg,char *func); /* It prints error message and exits
                                     the program. */
void Warning(char *msg,char *func); /* It prints warning message and
                                       leaves the routine. */
void Change(int *a, int *b); /* It changes content between a and b */

int  NCFgets(char *s, int m, FILE *f); /* It skips # comments */

/**
 * Gera um número inteiro aleatório no intervalo [low,high].
http://www.ime.usp.br/~pf/algoritmos/aulas/random.html
 **/
int RandomInteger (int low, int high);

#endif

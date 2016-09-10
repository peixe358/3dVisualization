#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "common.h"

/* Implementacoes vetoriais de uma imagem cinza 2D: inteira e double */

typedef struct _image { 
  int *val; /* brilho do pixel */
  int  ncols,nrows; /* dimensoes da imagem */
  int *tbrow; /* tabela de linhas */
} Image;

typedef struct _DImage{
  double *val;
  int ncols,nrows;
  int *tbrow;
} DImage;


/* Implementacoes matriciais */

typedef struct _grayimage {
  int   nx,ny;    /* dimensoes da imagem */
  int   **val;    /* matriz com os valores dos pixels */ 
  float dx,dy;    /* tamanho do pixel em unidades de comprimento */
  char  unid[10]; /* unidade de comprimento */
} GrayImage;

typedef struct _cor {
  int val[3]; /* R,G,B */
} Cor;

typedef struct _colorimage {
  int   nx,ny;    /* dimensoes da imagem */
  Cor **cor;      /* matriz com a cor dos pixels */ 
  float dx,dy;    /* tamanho do pixel em unidades de comprimento */
  char  unid[10]; /* unidade de comprimento */
} ColorImage;

typedef struct _medicalimage {
  int    nx,ny,nz; /* dimensoes da imagem */
  int ***val;      /* volume com os valores dos voxels */ 
  float  dx,dy,dz; /* tamanho do voxel em unidades de comprimento */
  char   unid[10]; /* unidade de comprimento */
} MedicalImage;

Image        *CreateImage(int ncols,int nrows);
void          DestroyImage(Image **img);
Image        *ReadImage(char *filename);
void          WriteImage(Image *img, char *filename);
Image        *MakeImage(char *data);
Image        *AddZeros(Image *img, int ncols, int nrows);
Image        *CreateCos(int ncols, int nrows, 
			float wx, float wy, 
			float phix, float phiy);
Image        *Abs(Image *img);
Image        *ImageMagnitude(Image *imgx, Image *imgy);

DImage       *CreateDImage(int ncols, int nrows);
void          DestroyDImage(DImage **dimg);

GrayImage    *CreateGrayImage(int nx, int ny); 
void          DestroyGrayImage(GrayImage **I); 
GrayImage    *ReadGrayImage(char *filename);
void          WriteGrayImage(GrayImage *I, char *filename); 

ColorImage   *CreateColorImage(int nx, int ny); 
void          DestroyColorImage(ColorImage **I);
ColorImage   *ReadColorImage(char *filename);
void          WriteColorImage(ColorImage *I, char *filename); 

MedicalImage *CreateMedicalImage(int nx, int ny, int nz); 
void          DestroyMedicalImage(MedicalImage **I);
MedicalImage *ReadMedicalImage(char *filename);
void          WriteMedicalImage(MedicalImage *I, char *filename); 

#endif

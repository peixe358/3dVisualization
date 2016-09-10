#ifndef _LINEAR_H_
#define _LINEAR_H_

#include "common.h"
#include "adjacency.h"
#include "kernel.h"
#include "image.h"

/* Calcula a convolução entre a imagem e o kernel, gerando uma imagem
   com dominio maior */

Image *Convolution(Image *img, Kernel *K);

/* Calcula a correlação entre a imagem e o kernel, gerando uma imagem
   com dominio maior */

Image *Correlation(Image *img, Kernel *K);

/* Calcula a filtragem linear entre a imagem e o kernel, gerando uma
   imagem com o mesmo dominio. Neste caso assume-se também que o
   kernel já está refletido. */

Image     *LinearFilter(Image *img, Kernel *K);
GrayImage *LinearFilter1(GrayImage *I, Kernel *K);

#endif

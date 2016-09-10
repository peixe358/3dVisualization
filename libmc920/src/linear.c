#include "linear.h"

Image *Convolution(Image *img, Kernel *K)
{
  int     dmax = MaximumDisplacement(K), xp, yp, xpi, ypi, pi, i; 
  int     q, xq, yq;
  Kernel *K1   = ReflectKernel(K); /* cria kernel refletido */
  Image  *conv = CreateImage(img->ncols+2*dmax+1, img->nrows+2*dmax+1);
  float   val;


  /* Para todo deslocamento de (xp,yp) de (-dmax,-dmax) até
     (img->ncols+dmax,img->nrows+dmax), calcula a soma da
     multiplicação entre a imagem e o kernel refletido. */

  for (yp=-dmax; yp <= img->nrows+dmax; yp++)  
    for (xp=-dmax; xp <= img->ncols+dmax; xp++){
      /* calcula coordenada na imagem de saida, pois esta deve ir de
	 (0,0) ate (img->ncols+2*dmax,img->nrows+2*dmax), e zera o
	 valor a ser acumulado. */
      xpi = xp+dmax; ypi = yp+dmax;
      pi  = xpi + conv->tbrow[ypi];
      val = 0.0;
      /* Para todo adjacente q de p no dominio da imagem de entrada,
	 calcula a soma da multiplicação entre a imagem de entrada e o
	 peso do adjacente. */
      for (i=0; i < K1->A->n; i++) {
	xq = xp + K1->A->adj[i].dx;
	yq = yp + K1->A->adj[i].dy;
	if ((xq >= 0)&&(xq < img->ncols)&&(yq >= 0)&&(yq < img->nrows)){
	  q = xq + img->tbrow[yq];
	  val += img->val[q]*K1->w[i];
	}
      }
      conv->val[pi] = ROUND(val);
    }

  DestroyKernel(&K1);
  return(conv);
}

Image *Correlation(Image *img, Kernel *K)
{
  int     dmax = MaximumDisplacement(K), xp, yp, xpi, ypi, pi, i; 
  int     q, xq, yq;
  Image  *corr = CreateImage(img->ncols+2*dmax+1, img->nrows+2*dmax+1);
  float   val;

  /* Para todo deslocamento de (xp,yp) de (-dmax,-dmax) até
     (img->ncols+dmax,img->nrows+dmax), calcula a soma da
     multiplicação entre a imagem e o kernel. */

  for (yp=-dmax; yp <= img->nrows+dmax; yp++)  
    for (xp=-dmax; xp <= img->ncols+dmax; xp++){
      /* calcula coordenada na imagem de saida, pois esta deve ir de
	 (0,0) ate (img->ncols+2*dmax,img->nrows+2*dmax), e zera o
	 valor a ser acumulado. */
      xpi = xp+dmax; ypi = yp+dmax;
      pi  = xpi + corr->tbrow[ypi];
      val = 0.0;
      /* Para todo adjacente q de p no dominio da imagem de entrada,
	 calcula a soma da multiplicação entre a imagem de entrada e o
	 peso do adjacente. */
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < img->ncols)&&(yq >= 0)&&(yq < img->nrows)){
	  q = xq + img->tbrow[yq];
	  val += img->val[q]*K->w[i];
	}
      }
      corr->val[pi] = ROUND(val);
    }

  return(corr);
}

Image *LinearFilter(Image *img, Kernel *K)
{
  int     p, xp, yp, i; 
  int     q, xq, yq;
  Image  *fimg = CreateImage(img->ncols, img->nrows);
  float   val;

  /* Para todo deslocamento de (xp,yp) de (0,0) até (I->nx,I->ny),
     calcula a soma da multiplicação entre a imagem e o kernel. */

  for (yp=0; yp < img->nrows; yp++)  
    for (xp=0; xp < img->ncols; xp++){
      p   = xp + fimg->tbrow[yp];
      val = 0.0;
      /* Para todo adjacente q de p no dominio da imagem de entrada,
	 calcula a soma da multiplicação entre a imagem de entrada e o
	 peso do adjacente. */
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < img->ncols)&&(yq >= 0)&&(yq < img->nrows)){
	  q = xq + img->tbrow[yq];
	  val += img->val[q]*K->w[i];
	}
      }
      fimg->val[p] = ROUND(val);
    }

  return(fimg);
}

GrayImage *LinearFilter1(GrayImage *I, Kernel *K)
{
  int xp,yp,xq,yq,i;
  float val;
  GrayImage *J=CreateGrayImage(I->nx,I->ny);

  /* Para todo deslocamento de (xp,yp) de (0,0) até (I->nx,I->ny),
     calcula a soma da multiplicação entre a imagem e o kernel. */

  for (yp=0; yp < J->ny; yp++) {
    for (xp=0; xp < J->nx; xp++) {
      val = 0.0;
      /* Para todo adjacente q de p no dominio da imagem de entrada,
	 calcula a soma da multiplicação entre a imagem de entrada e o
	 peso do adjacente. */
      for (i=0; i < K->A->n; i++) {
	xq = xp + K->A->adj[i].dx;
	yq = yp + K->A->adj[i].dy;
	if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) 
	  val += I->val[yq][xq]*K->w[i];
      }
      J->val[yp][xp] = ROUND(val);
    }
  }

  return(J);
}






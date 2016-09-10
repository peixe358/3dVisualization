#include "mc920.h"

Image *KernelToImage(Image *img, Kernel *K)
{
  int i, x, y, dxmin, dymin;
  float wsum;
  Image *J=CreateImage(img->ncols,img->nrows);

  wsum=0.0; dxmin=100; dymin=100;
  for (i=0; i < K->A->n; i++) {
    wsum += fabs(K->w[i]);
    if (K->A->adj[i].dx<dxmin)
      dxmin = K->A->adj[i].dx;
    if (K->A->adj[i].dy<dymin)
      dymin = K->A->adj[i].dy;
  }

  for (i=0; i < K->A->n; i++) {
    K->w[i] = 255*(K->w[i]/wsum);
  }
  for (i=0; i < K->A->n; i++) {
    x = K->A->adj[i].dx-dxmin;
    y = K->A->adj[i].dy-dymin;
    J->val[x+y*J->ncols] = ROUND(K->w[i]);
  }

  return(J);
}

int main(int argc, char **argv) {
  Image *img,*magview,*filt[3],*kernel[3],*fimg[3];
  Spectrum  *img_spec,*filt_spec[3],*fimg_spec[3];
  char filename[100];
  int i;

  if (argc!=2) {
    fprintf(stderr,"usage: fftdemo <image> \n");
    fprintf(stderr,"image: arquivo de imagem pgm\n");    
    exit (-1);
  }

  img = ReadImage(argv[1]);

  /* Calcula FFT da imagem e grava magnitude para visualizacao */

  img_spec  = FFT2D(img);
  magview   = ViewMagnitude(img_spec);
  WriteImage(magview,"imagem-magnitude.pgm");
  DestroyImage(&magview);

  /* Visualiza filtro Gaussiano da biblioteca */

  /* Kernel *K = CircularGaussian(15.0); */
  /* filt[0] = KernelToImage(img,K); */
  /* sprintf(filename,"imagem-do-filtro.pgm"); */
  /* WriteImage(filt[0],filename); */
  /* filt_spec[0] = FFT2D(filt[0]); */
  /* magview      = ViewMagnitude(filt_spec[0]); */
  /* sprintf(filename,"spectrum-do-filtro.pgm"); */
  /* WriteImage(magview,filename); */
  /* fimg_spec[0] = MultSpectrum(img_spec,filt_spec[0]); */
  /* fimg[0]      = InvFFT2D(fimg_spec[0]); */
  /* WriteImage(fimg[0],"imagem-suavizada.pgm"); */
  /* exit(0); */

  /* Gera imagem de um kernel Gaussiano/Sobel, adiciona zeros para
     ficar na resolucao da imagem, calcula sua transformada e grava
     magnitude para visualizacao. */

  kernel[0]  = MakeImage("3,3,1,2,1,2,4,2,1,2,1");   
  kernel[1]  = MakeImage("3,3,-1,-2,-1,0,0,0,1,2,1");   
  kernel[2]  = MakeImage("3,3,-1,0,1,-2,0,2,-1,0,1");   
  for (i=0; i < 3; i++) {
    filt[i]      = AddZeros(kernel[i],img->ncols-3,img->nrows-3);
    filt_spec[i] = FFT2D(filt[i]);
    magview      = ViewMagnitude(filt_spec[i]);
    sprintf(filename,"filter-%d-magnitude.pgm",i+1);
    WriteImage(magview,filename);
    DestroyImage(&magview);
    DestroyImage(&kernel[i]);
  }

  /* Aplica a filtragem na frequencia */

  for (i=0; i < 3; i++) {
    fimg_spec[i] = MultSpectrum(img_spec,filt_spec[i]);
    fimg[i]      = InvFFT2D(fimg_spec[i]);
  }
  WriteImage(fimg[0],"imagem-suavizada.pgm");

  Image *grad = ImageMagnitude(fimg[1],fimg[2]);
  WriteImage(grad,"realce-bordas.pgm");

  for (i=0; i < 3; i++) {
    DestroyImage(&fimg[i]);
    DestroySpectrum(&filt_spec[i]);
    DestroySpectrum(&fimg_spec[i]);
    DestroyImage(&filt[i]);
  }

  DestroyImage(&img);
  DestroyImage(&grad);
  DestroySpectrum(&img_spec);

  /* Crie imagem do cosseno */

  img      = CreateCos(256,256,32,8,0,0);
  WriteImage(img,"cosseno.pgm");
  img_spec = FFT2D(img);
  magview  = ViewMagnitude(img_spec);
  WriteImage(magview,"cosseno-magnitude.pgm");
  DestroyImage(&magview);


  return 0;
}


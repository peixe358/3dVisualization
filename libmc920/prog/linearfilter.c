#include "mc920.h"


int main(int argc, char **argv) {
  GrayImage *I,*J;
  Image     *I1,*J1;
  Kernel    *K;

  if ((argc!=3)&&(argc!=2)) {
    fprintf(stderr,"usage: linearfilter <image> <kernel>\n");
    fprintf(stderr,"image:  arquivo de imagem pgm\n");    
    fprintf(stderr,"kernel (opcional): arquivo com a lista de adjacentes e pesos\n");    
    exit (-1);
  }

  I  = ReadGrayImage(argv[1]);
  I1 = ReadImage(argv[1]);
  if (argc==3)
    K = ReadKernel2D(argv[2]);
  else
    K = CircularGaussian(15.0);
  J = LinearFilter1(I,K);
  WriteGrayImage(J,"filtered.pgm");

  J1 = LinearFilter(I1,K);
  WriteImage(J1,"filtered1.pgm");

  DestroyGrayImage(&I);
  DestroyGrayImage(&J);

  DestroyImage(&J1);
  J1 = Convolution(I1,K);
  WriteImage(J1,"convolution.pgm");

  DestroyImage(&J1);
  J1 = Correlation(I1,K);
  WriteImage(J1,"correlation.pgm");

  DestroyKernel(&K);
  DestroyImage(&I1);
  DestroyImage(&J1);

  return 0;
}


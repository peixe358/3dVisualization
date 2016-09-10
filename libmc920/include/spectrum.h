#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include "image.h"

typedef struct _spectrum {
  double *real;
  double *imag;
  int ncols,nrows;
  int *tbrow;
} Spectrum;


int       FFT(int dir, long nn, double *x, double *y);
Spectrum *FFT2D(Image *img);
Image    *InvFFT2D(Spectrum *spec);
DImage   *DInvFFT2D(Spectrum *spec);
Spectrum *CreateSpectrum(int ncols, int nrows);
void      DestroySpectrum(Spectrum **spec);
Image    *ViewMagnitude(Spectrum *spec);
Image    *ViewPhase(Spectrum *spec);
Spectrum *SpectrumMagnitude(Spectrum *spec);
Spectrum *SpectrumPhase(Spectrum *spec);
Spectrum *MultSpectrum(Spectrum *spec1, Spectrum *spec2);


#endif


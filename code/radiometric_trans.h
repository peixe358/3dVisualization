#ifndef RADIOMETRIC_TRANS_H

int clamp(int value, int min, int max);
GrayImage* calculateIntensity(GrayImage *I, int l1, int l2, int k1, int k2);
GrayImage* negative(GrayImage* I);
GrayImage* normalize(GrayImage* I, int H);
GrayImage* contrastAndBrightness(GrayImage* I, int contrast=50, int brightness=50, int H=4095);
int numberOfLabels(GrayImage *gray);
ColorImage* Gray2Color(GrayImage *gray, int H);
ColorImage* Gray2ColorLabelImage(GrayImage *gray, int H);
ColorImage* ConcatenateLabelImage(GrayImage *gray, ColorImage *label_image, int n);

#endif
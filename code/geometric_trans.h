#ifndef GEOMETRIC_TRANS_H
GrayImage* getPlanarImage(Point p1, Point p2, MedicalImage *image, int dx2=-1, int dy2=-1);
MedicalImage* reformatImage(Point p1, Point p2, MedicalImage *image, int dx2=-1, int dy2=-1);
void MedicalImage2GrayImages(MedicalImage *image, const char *output_dir);
GrayImage* maxIntensityProjection(MedicalImage *image, int theta_x, int theta_y);

#endif
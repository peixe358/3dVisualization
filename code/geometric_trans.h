#ifndef GEOMETRIC_TRANS_H
GrayImage* getPlanarImage(Point p1, Point p2, MedicalImage *image, int dx2=-1, int dy2=-1);
MedicalImage* reformatImage(Point p1, Point p2, MedicalImage *image, int dx2=-1, int dy2=-1);

#endif
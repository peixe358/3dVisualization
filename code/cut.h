#ifndef CUT_H


struct Vector3d {
   int x;
   int y;
   int z;

};

enum Type {AXIAL, CORONAL, SAGITAL};

GrayImage* get_slice_2d(MedicalImage *I, Vector3d input,  Type type, int slice);
void test();


#endif
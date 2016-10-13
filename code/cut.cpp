extern "C" {
#include "mc920.h"
}

#include <iostream>

#include "cut.h"


using namespace std;

GrayImage* get_slice_2d(MedicalImage *I, Vector3d input,  Type type, int slice){

   Vector3d v;
   GrayImage *output;

   if (type == AXIAL){
      v.x = abs(input.x) * I->nx;
      v.y = abs(input.y) * I->ny;
      v.z = abs(input.z) * I->nz;

      output = CreateGrayImage(I->nx, I->ny);
   }

   if (type == CORONAL){
      v.x = abs(input.x) * I->nx;
      v.y = abs(input.z) * I->nz;
      v.z = abs(input.y) * I->ny;
      
      output = CreateGrayImage(I->nx, I->nz);
   }


   if (type == SAGITAL){
      v.x = abs(input.z) * I->nz;
      v.y = abs(input.y) * I->ny;
      v.z = abs(input.x) * I->nx;

      output = CreateGrayImage(I->nz, I->ny);
   }

/*   if (input.x ^ input.y) {
      cout << "swap" << endl;
      int swap = v.y;
      v.y = v.x;
      v.x = swap;
   }
*/

   // Given a plan xy, a slice is extracted in z axis
   int i, j, fi, fj, opi, opj;

   if (input.x < 0) {  // -1
      i = v.x - 1;
      fi = -1; // need  to process until the 0 index (condition is !=, decreasing)
      opi = -1;
   }
   else {
      i = 0;
      fi = v.x;
      opi = 1;
   }


   if (input.y < 0) {  // -1
      j = v.y - 1;
      fj = -1; // need  to process until the 0 index (condition is !=, decreasing)
      opj = -1;
   }
   else {
      j = 0;
      fj = v.y;
      opj = 1;
   }


   //cout << "i, fi :" << i << " " << fi << endl;
   //cout << "j, fj :" << j << " " << fj << endl;

   //return 0;     


/*   if (input.x ^ input.y) {

      for (int it_i = i; it_i != fi; it_i += opi) {
         for (int it_j = j; it_j != fj; it_j += opj) {
            //cout << "(i, j) = " << it_i << " " << it_j << endl;
            if (type == AXIAL)
               cout << I->val[slice][it_j][it_i] << "  ";

            if (type == CORONAL){
               //cout <<  it_j << " " << slice << " " << it_i << endl;   
               cout << I->val[it_j][slice][it_i] << "  ";
            }
         }
         cout << endl;
      }
   }
   else {
*/





   for (int it_j = j; it_j != fj; it_j += opj) {
      for (int it_i = i; it_i != fi; it_i += opi) {
         //cout << "(i, j) = " << it_i << " " << it_j << endl;
         if (type == AXIAL)
            //cout << I->val[slice][it_j][it_i] << "  ";
            output->val[it_j][it_i] = I->val[slice][it_j][it_i];

         if (type == CORONAL){
            //cout <<  it_j << " " << slice << " " << it_i << endl;   
            //cout << I->val[it_j][slice][it_i] << "  ";
            output->val[it_j][it_i] = I->val[it_j][slice][it_i];
         }

         if (type == SAGITAL){
            //cout <<  it_j << " " << slice << " " << it_i << endl;   
            //cout << I->val[it_i][it_j][slice] << "  ";
            output->val[it_j][it_i] = I->val[it_i][it_j][slice];
         }
      }
   //cout << endl;
   }

  /*
   cout << endl << endl;
   for (int y=0; y< output->ny; ++y) {
      for (int x=0; x< output->nx; ++x) {
         cout << output->val[y][x] << " ";
      }
      cout << endl;
   }
   */


//}

   return output;

}



void test(){
   MedicalImage *image = CreateMedicalImage(3, 4, 5);

   int count = 1;
   for (int k = 0; k< image->nz; ++k) 
      for (int j = 0; j< image->ny; ++j) 
         for (int i = 0; i< image->nx; ++i) {
            image->val[k][j][i] = count++;
         }     

Vector3d input;
   input.x = 1;
   input.y = 1;
   input.z = 1;

   get_slice_2d(image, input,  AXIAL, 0);



   cout << " CORONAL" << endl;
   input.x = 1;
   input.y = 1;
   input.z = 1;

   get_slice_2d(image, input,  CORONAL, 0);


   cout << " SAGITAL" << endl;
   input.x = 1;
   input.y = 1;
   input.z = 1;

   get_slice_2d(image, input,  SAGITAL, 0);


}
extern "C" {
#include "mc920.h"
}


#include <iostream>
#include <string>
#include <math.h>
#include <set>


struct Vector3d {
   int x;
   int y;
   int z;

};

enum Type {AXIAL, CORONAL, SAGITAL};


using namespace std;



GrayImage *MedianFilter(GrayImage *I, AdjRel *A)
{
   int xp,yp,xq,yq,i,j,nelems,*cost=AllocIntArray(A->n);
   GrayImage *J=CreateGrayImage(I->nx,I->ny);
   GQueue *Q=CreateGQueue(256,A->n,cost);

   for (yp=0; yp < J->ny; yp++) {
      for (xp=0; xp < J->nx; xp++) {
         nelems=0;
         for (i=0; i < A->n; i++) {
            xq = xp + A->adj[i].dx;
            yq = yp + A->adj[i].dy;    
            if ((xq >= 0)&&(xq < J->nx)&&(yq >= 0)&&(yq < J->ny)) {
               cost[i]=I->val[yq][xq];
               InsertGQueue(&Q,i);
               nelems++;
            }        
         }
         j = 0;
         while (j <= nelems/2){
            i = RemoveGQueue(Q);
            j++;
         }
         xq = xp + A->adj[i].dx;
         yq = yp + A->adj[i].dy;
         J->val[yp][xp] = I->val[yq][xq];
         while (!EmptyGQueue(Q)){
            i = RemoveGQueue(Q);
         }      
      }
   }

   return(J);
}




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


int clamp(int value, int min, int max){

   if (value < min )
      return min;
   if (value > max)
      return max;

   return value;

}

GrayImage* calculateIntensity(GrayImage *I, int l1, int l2, int k1, int k2) {
   GrayImage *output = CreateGrayImage(I->nx, I->ny);

   for(int y = 0; y < I->ny; ++y) {
      for(int x = 0; x < I->nx; ++x) {
           //output->val[y][x] =  I->val[y][x];
         if (I->val[y][x]   < l1){
            output->val[y][x] = k1;
            continue;
         }
         
         if (I->val[y][x] >= l2) {
            output->val[y][x] = k2;
            continue;
         }
         if ((l2-l1) != 0)
            output->val[y][x] =  clamp((k2-k1) / (l2-l1) * (I->val[y][x])  + k1,0, k2); //     ((I->val[y][x] - level) / width) * (windownMax - windownMin); 
         else
            cout << "nao deveria ser zero" << endl;
      }
   }

   return output;
}


GrayImage* negative(GrayImage* I) {
   
   int max_value = MaximumValue(I);
   int min_value = MinimumValue(I);
   cout << "Negative : min " <<  min_value << "max " << max_value << endl;
   return calculateIntensity(I, min_value, max_value, max_value, min_value);

}


GrayImage* normalize(GrayImage* I, int H) {
   
   int max_value = MaximumValue(I);
   int min_value = MinimumValue(I);
   return calculateIntensity(I, min_value, max_value, 0, H);

}





GrayImage* contrastAndBrightness(GrayImage* I, int contrast=50, int brightness=50, int H=4095) {
   contrast = 100 - contrast;

   int minImage = MinimumValue(I);
   int maxImage = MaximumValue(I);

   int level = (1-brightness/100.)*(maxImage - minImage) + minImage;
   int width = (1-contrast/100.)*(maxImage - minImage);

   int windownMax = level + (width / 2);
   int windownMin = level - (width / 2);

   //cout << " min max" << windownMin << " " << windownMax << endl;
   
   if (H == 0)
      H = MaximumValue(I);

   return  calculateIntensity(I, windownMin, windownMax, 0, H);

/*   GrayImage *output = CreateGrayImage(I->nx, I->ny);


   for(int y = 0; y < I->ny; ++y) {
      for(int x = 0; x < I->nx; ++x) {
           //output->val[y][x] =  I->val[y][x];
         if (I->val[y][x]   <= windownMin) {
            output->val[y][x] = windownMin;
            continue;
         }

         if (I->val[y][x] > windownMin) {
           output->val[y][x] = windownMax; 
           continue;
         }

         output->val[y][x] =  clamp( ((I->val[y][x] - level) / width) * (windownMax - windownMin),0, 65535); 
      }
   }
   return output;
*/
}


int numberOfLabels(GrayImage *gray){
   set<int> labels;
   for(int y = 0; y < gray->ny; ++y) {
      for(int x = 0; x < gray->nx; ++x) {
         labels.insert(gray->val[y][x]);
      }
   }

  // cout << "labels size : " << labels.size() << endl;


 /*for (std::set<int>::iterator it=labels.begin(); it!=labels.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
*/

  return labels.size() -1 ;
}

ColorImage* Gray2Color(GrayImage *gray, int H) {
   ColorImage *output = CreateColorImage(gray->nx, gray->ny);
   
   for(int y = 0; y < gray->ny; ++y) {
      for(int x = 0; x < gray->nx; ++x) {
         double V;
         V = double(gray->val[y][x]) / double(H);
         //V = rand() % 100;
         //V += 1;
         //V /= 100;
         V = double(6.0-2.0) * V + 1.0;

         output->cor[y][x].val[RED]=    H * max(0, int((3.0 - abs(V-4.0) - abs(V-5.0))/2.0));// (int) V * H;//red
         output->cor[y][x].val[GREEN]=  H * max(0, int((4.0 - abs(V-2.0) - abs(V-4.0))/2.0)); //0;//(int)V * H;//green
         output->cor[y][x].val[BLUE]=   H * max(0, int((3.0 - abs(V-1.0) - abs(V-2.0))/2.0)); // 0;//(int)V * H;//blue
      }  
   }
   return output;
}



ColorImage* Gray2ColorLabelImage(GrayImage *gray, int H) {
   ColorImage *output = CreateColorImage(gray->nx, gray->ny);
   
   int labels = numberOfLabels(gray);   

   float delta =  1.0 / (labels);

   for(int y = 0; y < gray->ny; ++y) {
      for(int x = 0; x < gray->nx; ++x) {
         if (gray->val[y][x] != 0) {

            double V =  (gray->val[y][x] - 1) * delta ;
            //V = double(gray->val[y][x]) / double(H);
            //V = rand() % 100;
            //V += 1;
            //V /= 100;
            V = double(6.0-2.0) * V + 1.0;

            output->cor[y][x].val[RED]=    H * max(0, int((3.0 - abs(V-4.0) - abs(V-5.0))/2.0));// (int) V * H;//red
            output->cor[y][x].val[GREEN]=  H * max(0, int((4.0 - abs(V-2.0) - abs(V-4.0))/2.0)); //0;//(int)V * H;//green
            output->cor[y][x].val[BLUE]=   H * max(0, int((3.0 - abs(V-1.0) - abs(V-2.0))/2.0)); // 0;//(int)V * H;//blue
         }     
      }  
   }
   return output;
}


ColorImage* ConcatenateLabelImage(GrayImage *gray, ColorImage *label_image, int n) {
   ColorImage *output = CreateColorImage(gray->nx, gray->ny);
   
/*   cout << "max red " << MaximumIntensityColor(label_image, 0) << endl;
   cout << "max green " << MaximumIntensityColor(label_image, 1) << endl;
   cout << "max blue " << MaximumIntensityColor(label_image, 2) << endl;
   cout << "max gray intensity " << MaximumValue(gray) << endl; 
*/

   for(int y = 0; y < gray->ny; ++y) {
      for(int x = 0; x < gray->nx; ++x) {
         if (label_image->cor[y][x].val[RED] != 0 || label_image->cor[y][x].val[GREEN] != 0 || label_image->cor[y][x].val[BLUE] != 0) {
            if ( x % 2 == 0 && y % 2 == 0) {
               output->cor[y][x].val[RED]= label_image->cor[y][x].val[RED];
               output->cor[y][x].val[GREEN]= label_image->cor[y][x].val[GREEN];
               output->cor[y][x].val[BLUE]= label_image->cor[y][x].val[BLUE];
            }
            else {
               output->cor[y][x].val[RED]= int(gray->val[y][x] * 1.2);
               output->cor[y][x].val[GREEN]= int (gray->val[y][x] * 1.2);
               output->cor[y][x].val[BLUE]=  int (gray->val[y][x] * 1.2);
            }
            
/*               output->cor[y][x].val[RED]= int(0.2989 * float (gray->val[y][x]));
               output->cor[y][x].val[GREEN]=  int(0.5870 * float(gray->val[y][x]));
               output->cor[y][x].val[BLUE]=  int (0.1140 * float(gray->val[y][x]));
               */
         }
         else {
               output->cor[y][x].val[RED]= gray->val[y][x];
               output->cor[y][x].val[GREEN]= gray->val[y][x];
               output->cor[y][x].val[BLUE]= gray->val[y][x];

         }

      }
   }
   return output;
}





int main(int argc, char **argv) {
   
   test();

   if (argc != 5) {
      cout << "<input image> " << endl ;
      cout << "<output directory> " << endl ;
      cout << "<number of slice> " << endl ;
      exit (-1);
   }
   
   string input_image(argv[1]);
   string output_dir(argv[2]);
   int slice = atoi(argv[3]);
   int H = atoi(argv[4]);

   string extension(".ppm");

 
   MedicalImage *image, *label_image;

   cout << input_image <<endl;
   image = ReadMedicalImage( argv[1] );

   Vector3d input;
   input.x = 1;
   input.y = 1;
   input.z = 1;
   GrayImage *axial, *coronal, *sagital;


   /********** Extract Cuts ************************/

   axial = get_slice_2d(image, input,  AXIAL, slice);
   WriteGrayImage(axial,  (output_dir + string("axial") + extension).c_str());

   coronal = get_slice_2d(image, input,  CORONAL, slice);
   WriteGrayImage(coronal, (output_dir + string("coronal") + extension).c_str());

   sagital = get_slice_2d(image, input, SAGITAL, slice);
   WriteGrayImage(sagital, (output_dir + string("sagital") + extension).c_str());
   

   
   /********** Normalize and negative ***********************

   GrayImage *negative_axial =  negative(axial);
   WriteGrayImage(negative_axial,  (output_dir + string("axial_negative") + extension).c_str());

   GrayImage *normalize_axial =  normalize(axial,255);
   WriteGrayImage(normalize_axial,  (output_dir + string("axial_normalize_255") + extension).c_str());
    */


   /********** Contrast  and Brightness ************************/
   

   // BASE

   GrayImage *contrast_axial =  contrastAndBrightness(axial ,50, 50);
   WriteGrayImage(contrast_axial,  (output_dir + string("axial_50_50") + extension).c_str());

   GrayImage *contrast_coronal =  contrastAndBrightness(coronal ,50, 50);
   WriteGrayImage(contrast_coronal,  (output_dir + string("coronal_50_50") + extension).c_str());

   GrayImage *contrast_sagital =  contrastAndBrightness(sagital ,50, 50);
   WriteGrayImage(contrast_sagital,  (output_dir + string("sagital_50_50") + extension).c_str());
  
   // only a test
   int max_value = MaximumValue(contrast_axial);
   ColorImage* color = Gray2Color(contrast_axial, max_value);
   WriteColorImage(color, (output_dir + string("contrast_axial_color") + extension).c_str());

   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);



   // CONTRAST

   contrast_axial =  contrastAndBrightness(axial ,10, 50, H);
   WriteGrayImage(contrast_axial,  (output_dir + string("axial_10_50") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,10, 50, H);
   WriteGrayImage(contrast_coronal,  (output_dir + string("coronal_10_50") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,10, 50, H);
   WriteGrayImage(contrast_sagital,  (output_dir + string("sagital_10_50") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);




   contrast_axial =  contrastAndBrightness(axial ,100, 50, H);
   WriteGrayImage(contrast_axial,  (output_dir + string("axial_100_50") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,100, 50, H);
   WriteGrayImage(contrast_coronal,  (output_dir + string("coronal_100_50") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,100, 50, H);
   WriteGrayImage(contrast_sagital,  (output_dir + string("sagital_100_50") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);



   // BRIGHTNESS

   contrast_axial =  contrastAndBrightness(axial ,50, 10, H);
   WriteGrayImage(contrast_axial,  (output_dir + string("axial_50_10") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,50, 10, H);
   WriteGrayImage(contrast_coronal,  (output_dir + string("coronal_50_10") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,50, 10, H);
   WriteGrayImage(contrast_sagital,  (output_dir + string("sagital_50_10") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);




   contrast_axial =  contrastAndBrightness(axial ,50, 100, H);
   WriteGrayImage(contrast_axial,  (output_dir + string("axial_50_100") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,50, 100, H);
   WriteGrayImage(contrast_coronal,  (output_dir + string("coronal_50_100") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,50, 100, H);
   WriteGrayImage(contrast_sagital,  (output_dir + string("sagital_50_100") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);






   /********** Create Color Images ************************/
   label_image = ReadMedicalImage("/home/peixe/3dVisualization/libmc920/data/brain_label.scn");   

   // AXIAL
   GrayImage* label_image_axial = get_slice_2d(label_image, input,  AXIAL, slice);
   // maximum value from gray image
   max_value = MaximumValue(axial);

   // create mask using maximum gray intensity
   ColorImage *axial_label_color =   Gray2ColorLabelImage(label_image_axial, max_value);
   WriteColorImage(axial_label_color, (output_dir + string("axial_label_color") + extension).c_str());


   ColorImage * concatenateImage_axial = ConcatenateLabelImage(axial, axial_label_color, 0);
   WriteColorImage(concatenateImage_axial, (output_dir + string("axial_label_and_mask") + extension).c_str());


   // CORONAL

   GrayImage* label_image_coronal = get_slice_2d(label_image, input,  CORONAL, slice);
   // maximum value from gray image
   max_value = MaximumValue(coronal);

   // create mask using maximum gray intensity
   ColorImage *coronal_label_color =   Gray2ColorLabelImage(label_image_coronal, max_value);
   WriteColorImage(coronal_label_color, (output_dir + string("coronal_label_color") + extension).c_str());


   ColorImage * concatenateImage_coronal = ConcatenateLabelImage(coronal, coronal_label_color, 0);
   WriteColorImage(concatenateImage_coronal, (output_dir + string("coronal_label_and_mask") + extension).c_str());


   // SAGITAL

   GrayImage* label_image_sagital = get_slice_2d(label_image, input,  SAGITAL, slice);
   // maximum value from gray image
   max_value = MaximumValue(sagital);

   // create mask using maximum gray intensity
   ColorImage *sagital_label_color =   Gray2ColorLabelImage(label_image_sagital, max_value);
   WriteColorImage(sagital_label_color, (output_dir + string("sagital_label_color") + extension).c_str());


   ColorImage * concatenateImage_sagital = ConcatenateLabelImage(sagital, sagital_label_color, 0);
   WriteColorImage(concatenateImage_sagital, (output_dir + string("sagital_label_and_mask") + extension).c_str());





/********** Destroy images ************************/
   DestroyGrayImage(&axial);
   DestroyGrayImage(&coronal);
   DestroyGrayImage(&sagital);
   DestroyGrayImage(&label_image_axial);


   DestroyColorImage(&color);
   DestroyColorImage(&axial_label_color);
   DestroyColorImage(&concatenateImage_axial);

   return 0;
}


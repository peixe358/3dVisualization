extern "C" {
#include "mc920.h"
}

#include <iostream>
#include <set>


#include "radiometric_trans.h"

using namespace std;

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


GrayImage* contrastAndBrightness(GrayImage* I, int contrast, int brightness, int H) {
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

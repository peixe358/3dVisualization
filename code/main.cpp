extern "C" {
#include "mc920.h"
}

#include <iostream>
#include <string>
#include <math.h>
#include <set>

#include "cut.h"
#include "radiometric_trans.h"
#include "geometric_trans.h"


using namespace std;



/*GrayImage* getWireFrame(GrayImage *image, int theta_x, int theta_y){
}
*/



int main(int argc, char **argv) {
   
   //test(); test() funtion in cut.cpp

   if (argc != 5) {
      cout << "<Error - Check the Arguments> " << endl ;      
      cout << "<Input Image> " << endl ;
      cout << "<Output Directory> " << endl ;
      cout << "<Number of Slice> " << endl ;
      cout << "<Value to H,  pass 0 by default> " << endl ;
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
   WriteGrayImage(axial,  (output_dir + "/plane/" + string("axial") + extension).c_str());

   coronal = get_slice_2d(image, input,  CORONAL, slice);
   WriteGrayImage(coronal, (output_dir + "/plane/" + string("coronal") + extension).c_str());

   sagital = get_slice_2d(image, input, SAGITAL, slice);
   WriteGrayImage(sagital, (output_dir + "/plane/" + string("sagital") + extension).c_str());
   

   
   /********** Normalize and negative ***********************

   GrayImage *negative_axial =  negative(axial);
   WriteGrayImage(negative_axial,  (output_dir + string("axial_negative") + extension).c_str());

   GrayImage *normalize_axial =  normalize(axial,255);
   WriteGrayImage(normalize_axial,  (output_dir + string("axial_normalize_255") + extension).c_str());
    */


   /********** Contrast  and Brightness ************************/
   

   // BASE

   GrayImage *contrast_axial =  contrastAndBrightness(axial ,50, 50);
   cout << (output_dir + "/cortes/" + string("axial_50_50") + extension).c_str() << endl;
   WriteGrayImage(contrast_axial,  (output_dir + "/brightness_contrast/" + string("axial_50_50") + extension).c_str());

   GrayImage *contrast_coronal =  contrastAndBrightness(coronal ,50, 50);
   WriteGrayImage(contrast_coronal,  (output_dir + "/brightness_contrast/" + string("coronal_50_50") + extension).c_str());

   GrayImage *contrast_sagital =  contrastAndBrightness(sagital ,50, 50);
   WriteGrayImage(contrast_sagital,  (output_dir + "/brightness_contrast/" + string("sagital_50_50") + extension).c_str());
  
   // only a test
   int max_value = MaximumValue(contrast_axial);
   ColorImage* color = Gray2Color(contrast_axial, max_value);
   WriteColorImage(color, (output_dir + "/brightness_contrast/" + string("contrast_axial_color") + extension).c_str());

   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);



   // CONTRAST

   contrast_axial =  contrastAndBrightness(axial ,10, 50, H);
   WriteGrayImage(contrast_axial,  (output_dir + "/brightness_contrast/" + string("axial_10_50") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,10, 50, H);
   WriteGrayImage(contrast_coronal,  (output_dir + "/brightness_contrast/" + string("coronal_10_50") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,10, 50, H);
   WriteGrayImage(contrast_sagital,  (output_dir + "/brightness_contrast/" + string("sagital_10_50") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);




   contrast_axial =  contrastAndBrightness(axial ,100, 50, H);
   WriteGrayImage(contrast_axial,  (output_dir + "/brightness_contrast/" + string("axial_100_50") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,100, 50, H);
   WriteGrayImage(contrast_coronal,  (output_dir + "/brightness_contrast/" + string("coronal_100_50") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,100, 50, H);
   WriteGrayImage(contrast_sagital,  (output_dir + "/brightness_contrast/" + string("sagital_100_50") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);



   // BRIGHTNESS

   contrast_axial =  contrastAndBrightness(axial ,50, 10, H);
   WriteGrayImage(contrast_axial,  (output_dir + "/brightness_contrast/" + string("axial_50_10") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,50, 10, H);
   WriteGrayImage(contrast_coronal,  (output_dir + "/brightness_contrast/" + string("coronal_50_10") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,50, 10, H);
   WriteGrayImage(contrast_sagital,  (output_dir + "/brightness_contrast/" + string("sagital_50_10") + extension).c_str());
  
   DestroyGrayImage(&contrast_axial);
   DestroyGrayImage(&contrast_coronal);
   DestroyGrayImage(&contrast_sagital);




   contrast_axial =  contrastAndBrightness(axial ,50, 100, H);
   WriteGrayImage(contrast_axial,  (output_dir + "/brightness_contrast/" + string("axial_50_100") + extension).c_str());

   contrast_coronal =  contrastAndBrightness(coronal ,50, 100, H);
   WriteGrayImage(contrast_coronal,  (output_dir + "/brightness_contrast/" + string("coronal_50_100") + extension).c_str());

   contrast_sagital =  contrastAndBrightness(sagital ,50, 100, H);
   WriteGrayImage(contrast_sagital,  (output_dir + "/brightness_contrast/" + string("sagital_50_100") + extension).c_str());
  
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
   WriteColorImage(axial_label_color, (output_dir + "/color/" + string("axial_label_color") + extension).c_str());


   ColorImage * concatenateImage_axial = ConcatenateLabelImage(axial, axial_label_color, 0);
   WriteColorImage(concatenateImage_axial, (output_dir + "/color/" + string("axial_label_and_mask") + extension).c_str());


   // CORONAL

   GrayImage* label_image_coronal = get_slice_2d(label_image, input,  CORONAL, slice);
   // maximum value from gray image
   max_value = MaximumValue(coronal);

   // create mask using maximum gray intensity
   ColorImage *coronal_label_color =   Gray2ColorLabelImage(label_image_coronal, max_value);
   WriteColorImage(coronal_label_color, (output_dir + "/color/" + string("coronal_label_color") + extension).c_str());


   ColorImage * concatenateImage_coronal = ConcatenateLabelImage(coronal, coronal_label_color, 0);
   WriteColorImage(concatenateImage_coronal, (output_dir + "/color/" + string("coronal_label_and_mask") + extension).c_str());


   // SAGITAL

   GrayImage* label_image_sagital = get_slice_2d(label_image, input,  SAGITAL, slice);
   // maximum value from gray image
   max_value = MaximumValue(sagital);

   // create mask using maximum gray intensity
   ColorImage *sagital_label_color =   Gray2ColorLabelImage(label_image_sagital, max_value);
   WriteColorImage(sagital_label_color, (output_dir + "/color/" + string("sagital_label_color") + extension).c_str());


   ColorImage * concatenateImage_sagital = ConcatenateLabelImage(sagital, sagital_label_color, 0);
   WriteColorImage(concatenateImage_sagital, (output_dir + "/color/" + string("sagital_label_and_mask") + extension).c_str());


   // Extract planar slice from a medical image
   Point p1, p2;

   p1.x = image->nx / 2;
   p1.y = image->ny / 2;
   p1.z = 80;

   p2.x = p1.x + 1;
   p2.y = p1.y + 1;
   p2.z = p1.z + 1;

   GrayImage * planarImage = getPlanarImage(p1, p2, image);
   WriteGrayImage(planarImage,  (output_dir + "/planar_cut/" + string("Corte_Planar_1_1_1") + extension).c_str());
   DestroyGrayImage(&planarImage);


   p2.x = p1.x;
   p2.y = p1.y + 1;
   p2.z = p1.z + 1;

   planarImage = getPlanarImage(p1, p2, image);
   WriteGrayImage(planarImage,  (output_dir + "/planar_cut/" + string("Corte_Planar_0_1_1") + extension).c_str());
   DestroyGrayImage(&planarImage);


   p2.x = p1.x - 1;
   p2.y = p1.y + 1;
   p2.z = p1.z + 1;

   planarImage = getPlanarImage(p1, p2, image);
   WriteGrayImage(planarImage,  (output_dir + "/planar_cut/" + string("Corte_Planar_n1_1_1") + extension).c_str());
   DestroyGrayImage(&planarImage);

   // reformat image
   p1.x = image->nx / 2;
   p1.y = image->ny / 2;
   p1.z = 115;

   p2.x = p1.x + 130;
   p2.y = p1.y + 130;
   p2.z = p1.z + 130;

   MedicalImage* roi = reformatImage(p1, p2, image, image->nx, image->ny, 50);
   WriteMedicalImage(roi, (output_dir + "/reformat_image/" + string("Medical_ROI_Image") + ".scn").c_str());
   DestroyMedicalImage(&roi);

   /********** For test ************************/
   roi = ReadMedicalImage("/home/peixe/output_trabalho/current/reformat_image/Medical_ROI_Image.scn");   
   MedicalImage2GrayImages(roi, output_dir.c_str());



   /**** Max intensity projection  ***/
   MedicalImage *skull = ReadMedicalImage("/home/peixe/3dVisualization/libmc920/data/skull.scn");   
   for(int x=0; x<=360; x +=10){
      GrayImage *maxIntensity = maxIntensityProjection(skull, 45, x);
      WriteGrayImage(maxIntensity,  (output_dir + "/MIP/" + string("maxIntensity_") + std::to_string(x) + extension).c_str());
      DestroyGrayImage(&maxIntensity);
   }
/********** Destroy images ************************/
   DestroyGrayImage(&axial);
   DestroyGrayImage(&coronal);
   DestroyGrayImage(&sagital);
   DestroyGrayImage(&label_image_axial);
   //DestroyGrayImage(&maxIntensity);


   DestroyColorImage(&color);
   DestroyColorImage(&axial_label_color);
   DestroyColorImage(&concatenateImage_axial);

   return 0;
}


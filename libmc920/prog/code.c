#include "mc920.h"

Image *slice(MedicalImage *img, int cut, int pos){
// initialize slice
Image *img_slice=CreateImage(img->nx,img->ny);
int i, j;

  // iterate over image
  for(i=0;i<img->nx;i++)
  {
    for(j=0;j<img->ny;j++){
      img_slice->val[i][j] = img->val[i][j][pos];
    }
  }

  return(img_slice);
}

int main (){
MedicalImage *img;
Image *sliced;
img = ReadMedicalImage("/home/ricci/Documents/libmc920/data/skull.scn");

printf("%d\t%d\t%d\n", img->nx, img->ny, img->nz);

sliced = slice(img, 0, 120);

return 1;
}

// gtk 2.8 wx
// sudo apt-get install libpng // to run: -l png

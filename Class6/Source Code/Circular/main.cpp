////////////////////////////////////////////////////////////////////////////////
//                 TP 5 : Hough Transform (Circle version)                    //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"

using namespace cimg_library;

/*******************************************************************************

  MaxDetection: Compute local maximum
  ImgIn   : Input Image
  ImgOut  : Maximum map of ImgIn
  
*******************************************************************************/
void MaxDetection(CImg<> ImgIn, CImg<> &ImgOut)
{
 ImgOut.fill(0);
 CImgList<> ImgInGrad = ImgIn.get_gradient("xy",4);
 
 cimg_forXY(ImgOut,x,y)
 {
  float norm = sqrt(ImgInGrad[0](x,y)*ImgInGrad[0](x,y)+ImgInGrad[1](x,y)*ImgInGrad[1](x,y));

  float dx = ImgInGrad[0](x,y)/norm;
  float dy = ImgInGrad[1](x,y)/norm;  

  if(ImgIn(x,y)>ImgIn.linear_atXY(x+dx,y+dy) && ImgIn(x,y)>ImgIn.linear_atXY(x-dx,y-dy))
   ImgOut(x,y) = ImgIn(x,y);
 }
}

/*******************************************************************************



  DrawHoughCircle: Draw the circle of the Hough Transform and position in the accumulator
  
  ImgIn : Input Image where to draw the circles
  AccIn : Accumulator image where to draw the position of circles
  param : Circles in parametric form
  Rmin  : The minimum radius

*******************************************************************************/
void DrawHoughCircle(CImg<> &ImgIn,CImg<> &AccIn,CImg<> param, int Rmin)
{
 const unsigned char col1[3]={255,255,0}, col2[3]={0,0,0};
 cimg_forX(param,i)
 {
  ImgIn.
  draw_circle(param(i,0),param(i,1),param(i,2),col1,1.0f,0x00000000).
  draw_circle(param(i,0),param(i,1),param(i,2)+1,col2,1.0f,0x00000000).
  draw_circle(param(i,0),param(i,1),param(i,2)+2,col2,1.0f,0x00000000).
  draw_circle(param(i,0),param(i,1),param(i,2)-1,col2,1.0f,0x00000000).
  draw_circle(param(i,0),param(i,1),param(i,2)-2,col2,1.0f,0x00000000);

  CImg<> AccTemp = AccIn.get_slice(param(i,2)-Rmin);
  const float col3[1]={AccTemp.max()?AccTemp.max():1};
  AccTemp.
  draw_circle(param(i,0),param(i,1),7,col3,1.0f,0x00000000).
  draw_circle(param(i,0),param(i,1),8,col3,1.0f,0x00000000);
  
  cimg_forXY(AccTemp,x,y){AccIn(x,y,param(i,2)-Rmin)=AccTemp(x,y);}
 }
}

/*******************************************************************************

  MaxExtraction: Compute local maximum
  ImgIn  : Input Image
  param  : Extract the param.width() fist local max
  
*******************************************************************************/
void MaxExtraction(CImg<> Acc,CImg<>& param,int Rmin)
{
 param.fill(Rmin);

 // To complete ...

}

/*******************************************************************************

  CircleAccumulator: Compute an accumulator to detect the existence of circles
  
  ImgIn : Input Image
  Acc   : Accumulator in parametric space (Output)
  Alpha : Gradient Smoothing
  Sigma : Hough Transform smoothing
  Rmin  : The minimum radius

*******************************************************************************/

void CircleAccumulator(CImg<> ImgIn,CImg<> &Acc,float alpha, float sigma, int Rmin)
{
 // Initialization of accumulator
 Acc.fill(0);

// To complete ...

}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main(int argc,char **argv)
{
 cimg_usage("Retrieve command line arguments");
 const char* filename = cimg_option("-i","globule.bmp","Input image file");
 const double alpha   = cimg_option("-a",0.5,"Gradient smoothing");
 const double sigma   = cimg_option("-s",0.2,"Hough Transform smoothing");
 const int Rmin       = cimg_option("-rmin",5,"Minimum radius");
 const int Rmax       = cimg_option("-rmax",28,"Maximum radius");
 const int NbShape    = cimg_option("-n",20,"Number of the extracted parametric shapes");
 
 // Opening of filename
 CImg<> img(filename);
 
 // Accumulator Computation 
 int NbX = img.width();
 int NbY = img.height();
 int NbR = Rmax-Rmin+1;

 CImg<> acc(NbX,NbY,NbR);
 CircleAccumulator(img.get_channel(0),acc,alpha,sigma,Rmin);

 // Extract the local max of the accumulator
 CImg<> param(NbShape,4);
 MaxExtraction(acc,param,Rmin);

 // Draw circles 
 DrawHoughCircle(img,acc,param,Rmin);

 // Display
 int index_r = acc.depth()/2;
 CImg<> accToDisplay = acc.get_slice(index_r);
 float white[1]={accToDisplay.max()?accToDisplay.max():1}, black[1]={0};  
 accToDisplay.draw_text(10,10,"Courant radius : %d / %d",white,black,1,13,index_r+Rmin,Rmax);

 CImgDisplay dispSpatial(img,"Input Image");
 CImgDisplay accSpatial(accToDisplay,"Accumulator map (x0, y0, r)"); 

 while (!dispSpatial.is_closed() && !accSpatial.is_closed())
 {
  dispSpatial.wait(dispSpatial,accSpatial);

  if(accSpatial.is_keyPAGEUP() || accSpatial.is_keyPAGEDOWN())
  {
   if(accSpatial.is_keyPAGEDOWN())
    index_r = cimg::max(--index_r, 0);
   else
    index_r = cimg::min(++index_r, acc.depth()-1);
 
   accToDisplay = acc.get_slice(index_r);
   float white[1]={accToDisplay.max()?accToDisplay.max():1}, black[1]={0};  
   accToDisplay.draw_text(10,10,"Courant radius : %d / %d",white,black,1,13,index_r+Rmin,Rmax);
   accSpatial.display(accToDisplay);
  } 

  // When clicking on the vote window.
  if (accSpatial.button())
  {
   CImg<> win_param(1,3);
   win_param(0,0) = accSpatial.mouse_x();
   win_param(0,1) = accSpatial.mouse_y();
   win_param(0,2) = index_r+Rmin;
   
   DrawHoughCircle(img,acc,win_param,Rmin);

   accToDisplay = acc.get_slice(index_r);
   float white[1]={accToDisplay.max()?accToDisplay.max():1}, black[1]={0};  
   accToDisplay.draw_text(10,10,"Courant radius : %d / %d",white,black,1,13,index_r+Rmin,Rmax);

   dispSpatial.display(img);
   accSpatial.display(accToDisplay);
  }
 }
 return 0;
}

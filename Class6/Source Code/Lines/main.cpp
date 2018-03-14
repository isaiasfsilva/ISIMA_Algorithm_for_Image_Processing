/*

[2018] Isaias Faria Silva 
All Rights Reserved.
 
NOTICE: All information contained here in this file is the 
        property of Isaias Faria Silva
        If you want to use this, please put the contact 
        rights information.
 
PURPOSE:Practical activity of Algorithm for images
    Clermont-Auvergne University - Computer Science Institute
    ZZ3 - 2018 

DESCRIPTION: TP 6 - Images Analysis: Hough transform
    This is the main file of this project

Last update: 29 january 2018

TO DO:
  

*/

////////////////////////////////////////////////////////////////////////////////
//                          TP 5 : Hough Transform                            //
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

  float dx = 0.5*ImgInGrad[0](x,y)/norm;
  float dy = 0.5*ImgInGrad[1](x,y)/norm;  

  if(ImgIn(x,y)>ImgIn.linear_atXY(x+dx,y+dy) && ImgIn(x,y)>ImgIn.linear_atXY(x-dx,y-dy))
   ImgOut(x,y) = ImgIn(x,y);
 }
}

/*******************************************************************************

  DrawHoughLine: Draw the line of the Hough Transform and circle in the accumulator
  
  ImgIn : Input Image where to draw the lines
  AccIn : Accumulator image where to draw circles
  param : Line in parametric form

*******************************************************************************/
void DrawHoughLine(CImg<> &ImgIn,CImg<> &AccIn,CImg<> param,int NbTheta, int NbRho)
{
 const unsigned char col1[3]={255,255,0}, col2[3]={0,0,0};
 const float col3[1]={AccIn.max() ? AccIn.max() : 1 };

 cimg_forX(param,i)
 {
  const double
  rhomax   = sqrt(ImgIn.width()*ImgIn.width()+ImgIn.height()*ImgIn.height())/2.,
  thetamax = 2.*cimg::PI,
  rho   = param(i,1)*rhomax/NbRho,
  theta = param(i,0)*thetamax/NbTheta,
  x = ImgIn.width()/2 + rho*std::cos(theta),
  y = ImgIn.height()/2 + rho*std::sin(theta);

  const int
  x0 = (int)(x+1000*std::sin(theta)),
  y0 = (int)(y-1000*std::cos(theta)),
  x1 = (int)(x-1000*std::sin(theta)),
  y1 = (int)(y+1000*std::cos(theta));
 
  ImgIn.
  draw_line(x0,y0,x1,y1,col1,1.0f,0xF0F0F0F0).draw_line(x0,y0,x1,y1,col2,1.0f,0x0F0F0F0F).
  draw_line(x0+1,y0,x1+1,y1,col1,1.0f,0xF0F0F0F0).draw_line(x0+1,y0,x1+1,y1,col2,1.0f,0x0F0F0F0F).
  draw_line(x0,y0+1,x1,y1+1,col1,1.0f,0xF0F0F0F0).draw_line(x0,y0+1,x1,y1+1,col2,1.0f,0x0F0F0F0F);

  AccIn.
  draw_circle(param(i,0),param(i,1),7,col3,1.0f,0xF0F0F0F0).
  draw_circle(param(i,0),param(i,1),8,col3,1.0f,0xF0F0F0F0);
 }
}

/*******************************************************************************

  MaxExtraction: Compute local maximum
  ImgIn  : Input Image
  param  : Extract the param.width() fist local max
  
*******************************************************************************/
void MaxExtraction(CImg<> Acc,CImg<>& param)
{

 // Initialization of the detected lines
 param.fill(0);

 // To complete ....
}

/*******************************************************************************

  LineAccumulator: Compute an accumulator to detect the existence of lines
  
  ImgIn : Input Image
  Acc   : Accumulator in parametric space (Output)
  Alpha : Gradient Smoothing
  Sigma : Hough Transform smoothing
  Type  : 0: Vote by edge extraction
          1: Using gradient direction to reduce the number of votes

*******************************************************************************/

void LineAccumulator(CImg<> ImgIn,CImg<> &Acc,float alpha, float sigma, int type = 0)
{

         CImgList<> grad = ImgIn.get_gradient("xy",4);
     ImgIn[0].blur((float)alpha);
       ImgIn[1].blur((float)alpha);
       CImg<> NormGrad  = (grad[])

      cimg_forXY(grad,x,y) {
        double X = (double)x - ImgIn.width()/2;
        double Y = (double)y - ImgIn.height()/2;
        double gx = grad[0](x,y); 
        double gy = grad[1](x,y);
        double theta = std::atan2(gy,gx);
        double rho   = std::sqrt(X*X + Y*Y)*std::cos(std::atan2(Y,X) - theta);
       
      }

      Acc.blur((float)sigma);
 // Initialization of accumulator
 

 // To complete ....
}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main(int argc,char **argv)
{
 cimg_usage("Retrieve command line arguments");
 const char* filename = cimg_option("-i","../../Images/Lines/parrot.bmp","Input image file");
 const double alpha   = cimg_option("-a",2.0,"Gradient smoothing");
 const double sigma   = cimg_option("-s",1.5,"Hough Transform smoothing");
 const int type       = cimg_option("-t",1,"Hough Transform Type (0: Vote after edge detection; 1: Using the gradient direction)");
 const int NbShape    = cimg_option("-n",10,"Number of the extracted parametric shapes");
 
 // Opening of filename
 CImg<> img(filename);
 
 // Accumulator Computation 
 int NbTheta = 360;
 int NbRho   = (int)(sqrt(img.width()*img.width()+img.height()*img.height())/2.);
 CImg<> acc(NbTheta,NbRho);

 LineAccumulator(img.get_channel(0),acc,alpha,sigma,type);

 // Extract the local max of the accumulator
 CImg<> param(NbShape,3);
 MaxExtraction(acc,param);

 // Draw lines 
 DrawHoughLine(img,acc,param,NbTheta,NbRho);

 // Display
 CImgDisplay dispSpatial(img,"Input Image");
 CImgDisplay accSpatial(acc,"Accumulator map ($\rho$ and $\theta$)"); 

 while (!dispSpatial.is_closed() && !accSpatial.is_closed())
 {
  dispSpatial.wait(dispSpatial,accSpatial);

  // When clicking on the vote window.
  if (accSpatial.button())
  {
   CImg<> win_param(1,3);
   win_param(0,0) = accSpatial.mouse_x();
   win_param(0,1) = accSpatial.mouse_y();
   DrawHoughLine(img,acc,win_param,NbTheta,NbRho);

   dispSpatial.display(img);
   accSpatial.display(acc);
  }
 }
 return 0;
}

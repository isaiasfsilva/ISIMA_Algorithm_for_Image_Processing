////////////////////////////////////////////////////////////////////////////////
//                          TP 4 : Filtrage de Diffusion                      //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"

using namespace cimg_library;

/*******************************************************************************

                             Filtrage de diffusion

*******************************************************************************/
CImg<> PeronaMalik2D(CImg<float> imageIn, int nbiter, float delta_t, float lambda, int option)
{
 CImg<> imgOut= imageIn;
 
 
 for(int i=0; i<nbiter; i++){
 	CImgList<> NW = imgOut.get_gradient("xy",-1);
 	CImgList<> SE = imgOut.get_gradient("xy",+1);
 	cimg_forXY(imgOut, x,y){

 		float Gn, Gs,Ge,Gw;
		Gw = -NW[1](x,y);
		Ge =  SE[1](x,y);
		Gn = SE[0](x,y);
		Gs =-NW[0](x,y);

 		float Cn, Cs,Ce,Cw;
 		Cw = 1.0 / (1.0 + pow(lambda*Gw,2));
 		Ce = 1.0 / (1.0 + pow(lambda*Ge,2));
 		Cn = 1.0 / (1.0 + pow(lambda*Gn,2));
 		Cs = 1.0 / (1.0 + pow(lambda*Gs,2));

 		imgOut(x,y)+= delta_t*(Cw*Gw + Ce*Ge + Cn*Gn + Cs*Gs );

 	}

 }	
 

 
 return imgOut;
}
/*******************************************************************************

                   Filtrage de diffusion spatio-temporel

*******************************************************************************/
CImg<> PeronaMalik2D_T(CImg<> imageIn, int nbiter, float delta_t, float lambda, int option)
{
 CImg<> imgOut= imageIn;
 
 
 for(int i=0; i<nbiter; i++){
 	CImgList<> NW = imgOut.get_gradient("xyz",-1);
 	CImgList<> SE = imgOut.get_gradient("xyz",+1);
 	cimg_forXYZ(imgOut, x,y,z){

 		float Gn, Gs,Ge,Gw,GA,GB;
		Gw = -NW[1](x,y,z);
		Ge =  SE[1](x,y,z);
		Gn = SE[0](x,y,z);
		Gs =-NW[0](x,y,z);
		GA = SE[2](x,y,z);
		GB =-NW[2](x,y,z);

 		float Cn, Cs,Ce,Cw,Ca,Cb;
 		Cw = 1.0 / (1.0 + pow(lambda*Gw,2));
 		Ce = 1.0 / (1.0 + pow(lambda*Ge,2));
 		Cn = 1.0 / (1.0 + pow(lambda*Gn,2));
 		Cs = 1.0 / (1.0 + pow(lambda*Gs,2));
 		Ca = 1.0 / (1.0 + pow(lambda*GA,2));
 		Cb = 1.0 / (1.0 + pow(lambda*GB,2));

 		imgOut(x,y,z)+= delta_t*(Cw*Gw + Ce*Ge + Cn*Gn + Cs*Gs +Ca*GA + Cb*GB);

 	}

 }	
 

 
 return imgOut;
}
/*******************************************************************************

                                    Main

*******************************************************************************/
int main(int argc, char *argv[])
{
	printf("\n\tPARAMETERS: (int)NIterations* (float)LAMBDA*\n\n");
 	printf("running with %d iterations with lambda = %f \n",atoi(argv[1]),atof(argv[2]));
 // Ouverture de l'image "Lena.bmp"
 CImg<> img = CImg<>("../Images/TwoLeaveShop2front0330.bmp").channel(0);
 img.append(CImg<>("../Images/TwoLeaveShop2front0335.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0340.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0345.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0350.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0355.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0360.bmp").channel(0),'z');
 img.append(CImg<>("../Images/TwoLeaveShop2front0365.bmp").channel(0),'z');

 // Bruitage de l'image
 img.noise(-5);

 // Paramètres du filtre
 int   nbiter  = atoi(argv[1]);
 float delta_t = 0.3;
 float lambda  = atof(argv[2]);
 int   model   = 2;

 //Filtrage 2D de la quatrième image
 CImg<> imgout2D = PeronaMalik2D(img.get_slice(3),nbiter,delta_t,lambda,model);

 // Affichage
 CImgDisplay dispSpatial(img.get_slice(3),"Image Originale");
 CImgDisplay dispFilter2D(imgout2D,"Image Filtree en 2D");

 //Filtrage 2D+T
 CImg<> imgout2DT = PeronaMalik2D_T(img,nbiter,delta_t,lambda,model);

 // Affichage
 CImgDisplay dispFilter2DT(imgout2DT.get_slice(3),"Image Filtree en 2D+T");

  while (!dispSpatial.is_closed())
  {
	dispSpatial.wait();
  }
  return 0;
}

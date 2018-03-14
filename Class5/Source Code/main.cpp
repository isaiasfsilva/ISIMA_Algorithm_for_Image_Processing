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

DESCRIPTION: TP 5 - Flot optique
		This is the main file of this project

Last update: 22 january 2018

TO DO:
	Question B and C :/

*/

////////////////////////////////////////////////////////////////////////////////
//                          TP 6 : Flot optique                     //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include <math.h>

using namespace cimg_library;


CImg<> HornSchunck(CImg<> seq){
//output
 CImg<> field(seq.width(),seq.height(),1,2,0);

 //parameters
 double lambda=10;
 CImg<> u(seq.width(), seq.height());
 CImg<> v(seq.width(), seq.height());

 //get the gradient
 CImgList<> I = seq.get_gradient("xyz",4);

 //kernel
 CImg<double> W(3,3,1,1,1./9);

 for (int i = 0; i < 100; ++i){
	 
	 u= field.get_channel(0).get_convolve(W);
	 v=field.get_channel(1).get_convolve(W);

     cimg_forXY(field,x,y){
	 	field(x,y,0) = u(x,y)-I[0](x,y)*(I[0](x,y)*u(x,y) + I[1](x,y)*v(x,y)+I[2](x,y))/(lambda*lambda + I[0](x,y)*I[0](x,y) + I[1](x,y)*I[1](x,y));
	    field(x,y,1) =v(x,y)-I[1](x,y)*(I[0](x,y)*u(x,y) + I[1](x,y)*v(x,y)+I[2](x,y))/(lambda*lambda + I[0](x,y)*I[0](x,y) + I[1](x,y)*I[1](x,y));
	 } 	
 }
 return field;
}

CImg<> LucasKanade(CImg<> seq){
 CImg<> field(seq.width(),seq.height(),1,2);




 cimg_forXYC(field,x,y,v)
  field(x,y,v) = 0.01;

 return field;
}

CImg<> LucasKanade2(CImg<> seq)
{
 CImg<> field(seq.width(),seq.height(),1,2);

 cimg_forXYC(field,x,y,v)
  field(x,y,v) = 0.01;

 return field;
}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main()
{
 // Ouverture de l'image "Lena.bmp"
 CImg<> seq = CImg<>("../Images/rubic1.bmp").channel(0);
 seq.append(CImg<>("../Images/rubic2.bmp").channel(0),'z');

 CImg<> displacementHS  = HornSchunck(seq);
 CImg<> displacementLK  = LucasKanade(seq);
 CImg<> displacementLK2 = LucasKanade2(seq);

 // Affichage du champ résultat
 float color=500; unsigned int  sampling = 8; float  factor = 40; int  quiver_type = 0; float  opacity = 0.5;

 CImg<> imageHS = seq.get_slice(0).draw_quiver(displacementHS,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resHS_disp(imageHS,"Horn et Schunck");

 CImg<> imageLK = seq.get_slice(0).draw_quiver(displacementLK,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resLK_disp(imageLK,"Lucas et Kanade");

 CImg<> imageLK2 = seq.get_slice(0).draw_quiver(displacementLK2,&color,opacity,sampling,factor,quiver_type);
 CImgDisplay resLK2_disp(imageLK2,"Lucas et Kanade avec gestion des valeurs propres");

 while (!resLK_disp.is_closed())
 {

  resLK_disp.wait();
 }
  return 0;
}

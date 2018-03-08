////////////////////////////////////////////////////////////////////////////////
//              TP FINAL : Face detection using active contours               //
////////////////////////////////////////////////////////////////////////////////


/*


[2017] Isaias Faria Silva and Paula Metzker Soares
All Rights Reserved.
 
NOTICE: All information contained here in this file is the 
        property of Isaias Faria Silva and Paula Metzker Soares
        If you want to use this, please put the contact 
        rights information.
 
PURPOSE:Practical activity of Algorithm for images
		Clermont-Auvergne University - Computer Science Institute
		ZZ3 - 2018 March 
DESCRIPTION:
		This is the main file of this project

This code is also available on GitHub
	Link: https://goo.gl/qTDxar

Last update: 26th February 2017




Log activity:
	- 26/Feb/2018
		Calcul of Level Sets (r, g and I variables)
		...



*/




#include "CImg.h"
#include <iostream>
#include <cmath>

#define MAX(a,b) (((a)<(b)) ? (b) : (a) )
#define MIN(a,b) (((a)<(b)) ? (a) : (b) )


using namespace cimg_library;


/*******************************************************************************

     Calcul de la position approximative du contour à partir de la carte
   levelset (psi): isocontour de valeur 0. On recherche donc l'ensemble des
    pixels possédant un voisin de signe opposé (interface entre domaine <0
                                et domaine >0)

*******************************************************************************/
CImg<float> ExtractContour(CImg<float> LevelSet)
{
 CImg<float> Contour(LevelSet.width(),LevelSet.height(),1,1);
 Contour.fill(0);

 CImg_3x3(I,float);
 cimg_for3x3(LevelSet,x,y,0,0,I,float)
 {
  if(Icc*Icp<=0 || Icc*Icn<=0 || Icc*Ipc<=0 || Icc*Inc<=0)
   Contour(x,y) = 1;
 }
 return Contour;
}

/*******************************************************************************

                     Affichage d'un contour dans une image

*******************************************************************************/
void DrawContour(CImg<float>* imgIn,CImg<float> Contour)
{
 const float color = 2*imgIn->max();

 cimg_forXY(Contour,x,y)
 {
  if(Contour(x,y) == 1)
   imgIn->draw_point(x,y,0,&color,0.7);
 }
}

/*******************************************************************************

     Initialisation du LevelSet (psi) à l'aide de la distance euclidienne
    signée. Le contour initial est un cercle de centre (x0,y0) et de rayon r

*******************************************************************************/
void InitLevelSet(CImg<float>* imgIn, int x0,int y0, int r)
{
 cimg_forXY(*imgIn,x,y)
 {
   float buff = sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0))-r;
   if(buff>0){
   	(*imgIn)(x,y) = -10000;
   }else if(buff<0){
   	(*imgIn)(x,y) = 10000;
   }else{
   	 (*imgIn)(x,y) = 0;
   }

 }
}

/*******************************************************************************

      Algorithme de propagation d'un contour implicite (modéle géodésique)

*******************************************************************************/
void Propagate(CImg<float> imgIn, CImg<float>* LevelSet){

	//Commom variables
 CImg<float> r;
 CImg<float> g;
 CImg<float> I;
 CImg<> imgDraw = imgIn;
 // resizing
 r.resize(imgIn.width(),imgIn.height());
 g.resize(imgIn.width(),imgIn.height());
 I.resize(imgIn.width(),imgIn.height());

 cimg_forXY((imgIn),x,y){
   //Calc of r
   // r = R / (R+G+B)
   r(x,y) = imgIn(x,y,0,0) / (imgIn(x,y,0,0) + imgIn(x,y,0,1) + imgIn(x,y,0,2));

   //Calc of g
   // g = G / (R+G+B)
   g(x,y) = imgIn(x,y,0,1) / (imgIn(x,y,0,0) + imgIn(x,y,0,1) + imgIn(x,y,0,2));


   //Calc of I
   // I = (0.2126*R + 0.7152*G + 0.0722*B) 
   I(x,y) = (0.2126*imgIn(x,y,0,0) + 0.7152*imgIn(x,y,0,1) + 0.0722* imgIn(x,y,0,2));

 }


	imgDraw = imgIn;
	CImg<float> Contour = ExtractContour(*LevelSet);
	DrawContour(&imgDraw, Contour);
	CImgDisplay dispTemp(imgDraw,"Before everything");

	while (!dispTemp.is_closed()){
		dispTemp.wait();
	}



	//fixed parameters
	float r_     = 117.588;
	float g_     = 79.064;
	float lamb_1 = 5;	
	float lamb_2 = 15;
	float mi     = 1;
	float v      = 0;

	int   nbiter = 1000;
	int IterStop = 10;

	

	for(int it = 0; it < nbiter ; ++it){
		
		//Calc of the constant C
		int nb      =0;
		float buffer=0.0;
		cimg_forXY((*LevelSet),x,y){
			if((*LevelSet)(x,y)<0.0){
				buffer+=I(x,y);
				nb++;
			}
		}

		float c = buffer/nb;
		// end of C

			CImgList<> G_forw = (*LevelSet).get_gradient("xy",0);
			
			CImg<> Dx = G_forw[0];
	  		CImg<> Dy = G_forw[1];

	  		G_forw = Dx.get_gradient("xy",0);
	  		CImg<> Dxx = G_forw[0];
	  		CImg<> Dxy = G_forw[1];

	  		G_forw = Dy.get_gradient("xy",0);
	  		CImg<> Dyy = G_forw[1];



		cimg_forXY((*LevelSet),x,y){
			
			float DIV   = Dxx(x,y)*Dy(x,y)*Dy(x,y) - 2*Dx(x,y)*Dy(x,y)*Dxy(x,y) + Dyy(x,y)*Dx(x,y)*Dx(x,y);
			DIV        /= pow(Dx(x,y)*Dx(x,y) + Dy(x,y)*Dy(x,y), 1.5) + 0.0001;

			float buff  = mi*DIV;
			buff       -= v;
			buff       -= lamb_1*( pow(r_-r(x,y),2) + pow(g_-g(x,y),2));
			buff       += lamb_2 * pow(I(x,y) - c,2);

			(*LevelSet)(x,y) = (*LevelSet)(x,y) + buff;

		}

		if(!(it % IterStop)){
			imgDraw = imgIn;
			CImg<float> Contour = ExtractContour(*LevelSet);
			DrawContour(&imgDraw, Contour);
			CImgDisplay dispTemp(imgDraw,"Segment Image"+it);

			while (!dispTemp.is_closed()){
				dispTemp.wait();
			}
		}


	}
 	



/*










 int   nbiter  = 100;   // Nombre d'itération
 float delta_t = 1.5;      // Pas temporel delta t  == courbe doit etre dedans de lobjet sinon la courbe diverge
 float alpha   = 0.05;  
 float beta    = 0.8;    // Pondération du terme d'advection
 float ballon  = 0; //-0.01;  // Force ballon == vitesse constant qui force la convergence




 CImgList<> ImgInGrad = imgIn.get_gradient("xy",4);
 CImg<> imgDraw = imgIn;
 int IterStop = 100;
 
 CImg<> g_Grad(imgIn.width(),imgIn.height(),1,1,0);
 
 cimg_forXY(g_Grad,x,y){
	 g_Grad(x,y) = -1.0/(1.0 + ImgInGrad[0](x,y)*ImgInGrad[0](x,y) + ImgInGrad[1](x,y)*ImgInGrad[1](x,y));
	}
	
	CImgList<> d_g = g_Grad.get_gradient("xy",4);
	
	CImgDisplay dispG(imgDraw,"Energie du contour");

	 while (!dispG.is_closed())
	 {
	  dispG.wait();
	 }
 
  for(int it = 0; it < nbiter ; ++it){
	  
	 CImgList<> G_back = (*LevelSet).get_gradient("xy",-1);
	 CImgList<> G_forw = (*LevelSet).get_gradient("xy",1);
	  
	  CImg<> Dpx = G_forw[0];
	  CImg<> Dnx = G_back[0];
	  CImg<> Dpy = G_forw[1];
	  CImg<> Dny = G_back[1];
	  	  
  cimg_forXY((*LevelSet),x,y)
  {
	  float dp = sqrt((MAX(0.0,Dnx(x,y)))*(MAX(0.0,Dnx(x,y)))+(MIN(0.0, Dpx(x,y)))*(MIN(0.0, Dpx(x,y))) + (MAX(0.0,Dny(x,y)))*(MAX(0.0,Dny(x,y)))+(MIN(0.0, Dpy(x,y)))*(MIN(0.0, Dpy(x,y))) );
	  float dn = sqrt((MAX(0.0,Dpx(x,y)))*(MAX(0.0,Dpx(x,y)))+(MIN(0.0, Dnx(x,y)))*(MIN(0.0, Dnx(x,y))) + (MAX(0.0,Dpy(x,y)))*(MAX(0.0,Dpy(x,y)))+(MIN(0.0, Dny(x,y)))*(MIN(0.0, Dny(x,y))) );
	  
	  float u = d_g[0](x,y);
	  float v = d_g[1](x,y);
	  
	  float Fprop = MAX(0.0,g_Grad(x,y)) * dp + MIN(0.0,g_Grad(x,y)) * dn;
	  float Fadv = MAX(0.0,u)* Dnx(x,y) + MIN(0.0,u)* Dpx(x,y) + MAX(0.0,v)* Dny(x,y) + MIN(0.0,v)* Dpy(x,y);
	  
	  float speed = alpha*Fprop + beta*Fadv;
	 (*LevelSet)(x,y) = (*LevelSet)(x,y) - delta_t*speed;
  }
  
  if(!(it % IterStop)){
	  imgDraw = imgIn;
	  CImg<float> Contour = ExtractContour(*LevelSet);
	  DrawContour(&imgDraw, Contour);
	  CImgDisplay dispTemp(imgDraw,"Image Segmentée");

	 while (!dispTemp.is_closed())
	 {
	  dispTemp.wait();
	 }
	}	 
 }*/


}

/*******************************************************************************

                                      main

*******************************************************************************/
int main(int argc, char *argv[]){

 
 // Opening file
 CImg<float> img = CImg<float>("./Images/tests/1.bmp");



 // Définition d'un contour initial circulaire
 int x0 = img.width()/2;
 int y0 = img.height()/2;
 int r  = img.height()/4;

 CImg<float> levelset(img.width(),img.height(),1,1);
 InitLevelSet(&levelset,x0,y0,r);

 // Propagation du contour
 Propagate(img,&levelset);

 // Extraction du résultat
 CImg<float> Contour = ExtractContour(levelset);

 // Tracé du résultat dans l'image
 DrawContour(&img,Contour);

 // Affichage finale
 CImgDisplay dispSpatial(img,"Image Segmentée");

 while (!dispSpatial.is_closed())
 {
  dispSpatial.wait();
 }
 return 0;
}

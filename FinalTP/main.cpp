////////////////////////////////////////////////////////////////////////////////
//              TP FINAL : Face detection using active contours               //
////////////////////////////////////////////////////////////////////////////////


/*
[2018] Isaias Faria Silva and Paula Metzker Soares
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
Last update: 26th February 2018
Log activity:
	- 26/Feb/2018
		Calcul of Level Sets (r, g and I variables)
		...
Best coefficients:
	Lambda1 = 0.001
	Lambda2 = 0.02
	Mi      = 0.0005
*/




#include "CImg.h"
#include <iostream>
#include <cmath>
#include <unistd.h>
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
   (*imgIn)(x,y) = r - sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));

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

 	if(imgIn(x,y,0,0) == imgIn(x,y,0,1) == imgIn(x,y,0,2) == 0){
   	 	r(x,y) =  g(x,y) = 0.0;
   }else{
	   //Calc of r
	   // r = R / (R+G+B)
	   r(x,y) = imgIn(x,y,0,0) / (imgIn(x,y,0,0) + imgIn(x,y,0,1) + imgIn(x,y,0,2)+1);

	   //Calc of g
	   // g = G / (R+G+B)
	   g(x,y) = imgIn(x,y,0,1) / (imgIn(x,y,0,0) + imgIn(x,y,0,1) + imgIn(x,y,0,2) +1);
   }



   //Calc of I
   // I = (0.2126*R + 0.7152*G + 0.0722*B) 
   I(x,y) = (0.2126*imgIn(x,y,0,0) + 0.7152*imgIn(x,y,0,1) + 0.0722* imgIn(x,y,0,2));

 }


	imgDraw = imgIn;
	CImg<float> Contour = ExtractContour(*LevelSet);
	DrawContour(&imgDraw, Contour);
	CImgDisplay dispTemp(imgDraw,"Before everything");

	usleep(1000000);
	dispTemp.close();



	//fixed parameters
	float r_     = 117.588;
	float g_     = 79.064;
	float lamb_1 = 0.1; 	
	float lamb_2 =  0.4; 	
	float mi =  0.5; 		
	float v      = 0;

	int   nbiter = 10000;
	int IterStop = 100;
	float speed  = 1.5;
	

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
		float c = 0;
		if(nb!=0)
			c = buffer/nb;
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

			(*LevelSet)(x,y) = (*LevelSet)(x,y) + buff*speed;

		}

		if(!(it % IterStop)){
			imgDraw = imgIn;
			CImg<float> Contour = ExtractContour(*LevelSet);
			DrawContour(&imgDraw, Contour);
			CImgDisplay dispTemp(imgDraw,"Segment Image"+it);
			usleep(700000);
			
		}
	}
}

/*******************************************************************************
                                      main
*******************************************************************************/
int main(int argc, char *argv[]){

 if(argc!=2){
 	std::cout << "Input file error. The picture path must be informed as the first input" << std::endl;
 	exit(0);
 }

 // Opening file
 CImg<float> img = CImg<float>(argv[1]);


 // Définition d'un contour initial circulaire
 int x0 = img.width()/2;
 int y0 = img.height()/2;
 int r  = img.width()/2;

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

////////////////////////////////////////////////////////////////////////////////
//                       TP 7 : Contours actifs implicites                    //
////////////////////////////////////////////////////////////////////////////////

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
  (*imgIn)(x,y) = sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0))-r;
 }
}

/*******************************************************************************

      Algorithme de propagation d'un contour implicite (modéle géodésique)

*******************************************************************************/
void Propagate(CImg<float> imgIn, CImg<float>* LevelSet)
{
	// phi t+1 (i,j) = phi t (i,j) - delta t /// alpha*T{prop}+ beta*T{adv}+ [not in this algo]gamma*T{cur}+ ballon
 int   nbiter  = 4000;   // Nombre d'itération
 float delta_t = 1.5;      // Pas temporel delta t  == courbe doit etre dedans de lobjet sinon la courbe diverge
 float alpha   = 0.05;   // Pondération du terme de propagation
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
 }
}

/*******************************************************************************

                                      main

*******************************************************************************/
int main(int argc, char *argv[])
{
 // Ouverture de l'image "char256.bmp"
 CImg<float> img = CImg<float>("../Images/char256.bmp").channel(0);
 img.resize(img.width()/2,img.height()/2);

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

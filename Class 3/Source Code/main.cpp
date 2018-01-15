////////////////////////////////////////////////////////////////////////////////
//                          TP 3 : Filtrage Fréquentiel                       //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include <math.h>

using namespace cimg_library;

/*******************************************************************************

    Calcul d'une image composee du module de image1 et de la phase de image2

*******************************************************************************/
CImg<> Melange (CImg<> imageIn1,CImg<> imageIn2)
{
 CImg<> imgOut(imageIn1.width(),imageIn1.height(),1,1,0);

 
CImgList<> F = imageIn1.get_FFT(false); //we will use just the first coordinate
CImgList<> G = imageIn2.get_FFT(false);//we will use just the seecond coordinate



CImg<> Mod1 = (F[0].get_mul(F[0]) + F[1].get_mul(F[1])).get_sqrt();
CImg<> Arg = G[1].get_atan2(G[0]);


CImgList<> h(2);
h[0] = Mod1.get_mul(Arg.get_cos());
h[1] = Mod1.get_mul(Arg.get_sin());

CImgList<> h2 = h.get_FFT(true);
imgOut = h2[0];
 

 return imgOut.normalize(0,255);
}

/*******************************************************************************

                        Filtrage d'une image tramee

*******************************************************************************/
CImg<> Detramage (CImg<> imageIn)
{
 CImg<> imgOut(imageIn.width(),imageIn.height(),1,1,0);

 return imgOut;
}

/*******************************************************************************

      Filtrage Gaussien d'une image (avec un ecart-type spatial de 'sigma')

*******************************************************************************/
CImg<> FiltrageGaussien (CImg<> imageIn, float sigma)
{
 CImg<> imgOut(imageIn.width(),imageIn.height(),1,1,0);
 CImgList<> F = imageIn.get_FFT(false); //we will use just the first coordinate

 
CImg<> gauss = imgOut.draw_gaussian(imageIn.width()/2,imageIn.height()/2, 80 ,sigma );
 
 return imgOut;
}

/*******************************************************************************

                                    Main

*******************************************************************************/
int main()
{
 /*
 ///////////////////////////////////////////////////////////////////////////////
 //                        Question 1: Melange d'image                        //
 ///////////////////////////////////////////////////////////////////////////////

 // Ouverture des deux images
 CImg<> imgClown("../Images/clown.bmp");
 CImg<> imgGatlin("../Images/gatlin.bmp");

 // Resultat du melange

 CImg<> imgRes1 = Melange (imgClown.channel(0),imgGatlin.channel(0));

 // Affichage du resultat
 CImgDisplay dispClown(imgClown,"Question 1 : Image de Clown (1/3)");
 CImgDisplay dispGatlin(imgGatlin,"Question 1 : Image de Gatlin (2/3)");
 CImgDisplay dispRes1(imgRes1,"Question 1 : Image Melangee (3/3)");



 ///////////////////////////////////////////////////////////////////////////////
 //                        Question 2: Détramage                              //
 ///////////////////////////////////////////////////////////////////////////////

 // Ouverture de l'image "lena_trame.bmp"
 CImg<> imgLenaTramee("../Images/lena_trame.bmp");

 // Resultat du détramage
 CImg<> imgRes2 = Detramage (imgLenaTramee.channel(0));

 // Affichage du resultat
 CImgDisplay dispTramee(imgLenaTramee,"Question 2 : Image de Lena Tramee (1/2)");
 CImgDisplay dispDetramee(imgRes2,"Question 2 : Image de Lena Detramee (2/2)");
*/

 ///////////////////////////////////////////////////////////////////////////////
 //                        Question 3: Filtrage Gaussien                      //
 ///////////////////////////////////////////////////////////////////////////////

 // Ouverture de l'image "lena.bmp"
 CImg<> imgLena("../Images/lena.bmp");

 // Ajout d'un bruit additif Gaussien
 imgLena.noise(-5);

 // Résultat du détramage
 float sigma = 1;
 CImg<> imgRes3 = FiltrageGaussien (imgLena.channel(0),sigma);

 // Affichage du resultat
 CImgDisplay dispBruitee(imgLena,"Question 3 : Image de Lena Bruitee (1/2)");
 CImgDisplay dispDebruitee(imgRes3,"Question 3 : Image de Lena Debruitee (2/2)");


 while (!dispClown.is_closed())
 {
  dispClown.wait();
 }
 return 0;
}

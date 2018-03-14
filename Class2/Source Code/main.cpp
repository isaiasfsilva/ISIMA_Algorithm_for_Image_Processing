/*

[2017] Isaias Faria Silva 
All Rights Reserved.
 
NOTICE: All information contained here in this file is the 
        property of Isaias Faria Silva
        If you want to use this, please put the contact 
        rights information.
 
PURPOSE:Practical activity of Algorithm for images
		Clermont-Auvergne University - Computer Science Institute
		ZZ3 - 2017 december 

DESCRIPTION: TP 2 - Filtering in the spatial domain
		This is the main file of this project

Last update: 20 december 2017
*/


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "CImg.h"


using namespace cimg_library;
using namespace std;


/******************************************
Calcul de la phase du gradient de l'image imageIn
******************************************/
CImg<float> Phase (CImg<unsigned char> imageIn)
{	

 CImg<double> res(imageIn.width(),imageIn.height(),1,1,0);
 res = imageIn;

 CImg<> mask_x = CImg<>(3,3).fill(-1,-1,-1,0,0,0,1,1,1);
 CImg<> mask_y = CImg<>(3,3).fill(-1,0,1,-1,0,1,-1,0,1);

 CImg<double> res_y = imageIn.get_convolve(mask_y);
 res.convolve(mask_x);

 res.atan2(res_y);

 return res;  
}

/******************************************
Calcul d'un sigma_filtre sur l'image imageIn
******************************************/
CImg<float> sigmaFiltre (CImg<unsigned char> imageIn)
{
 CImg<double> res(imageIn.width(),imageIn.height(),1,1,0);
 res = imageIn;
 CImg_3x3(I,float); 
 cimg_for3x3(res, x,y,0,0,I,float){
 	res(x,y) = (1.0/9)*(Ipp+Icp+Inp+Icp+Icc+Icn+Inp+Inc+Inn)/((abs(Icc - (Ipp+Icp+Inp+Icp+Icc+Icn+Inp+Inc+Inn)))+1);


 }
 return res;  
}

/******************************************
Main
******************************************/
int main(int argc,char **argv) 
{ 
	cimg_usage("TP2 Imagerie - filtrage spatial");

	const string name_i   = cimg_option("-i","../Images/im.bmp","Image");

	CImg<unsigned char> image(name_i.c_str()); 
	
	CImg<float> resultPh = Phase(image);

	CImg<float> resultSg = sigmaFiltre(image);

	CImgDisplay main_disp(image,"Visualisation");
        CImgDisplay resP_disp(resultPh,"Phase du Gradient");
        CImgDisplay resS_disp(resultSg,"Sigma_filtre de l'image");
	
        while ( !main_disp.is_closed() ) 
	{ 
		main_disp.wait(); 
	}


return 0; 
}

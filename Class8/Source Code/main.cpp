////////////////////////////////////////////////////////////////////////////////
//                       TP 8 : Classification d'attributs                    //
////////////////////////////////////////////////////////////////////////////////

#include "CImg.h"
#include <iostream>

using namespace cimg_library;


/*******************************************************************************

                  Calcul de la fonction d'extraction d'attributs

*******************************************************************************/
CImg<> AttributsExtraction(CImg<> img)
{
 int p=2; // Nombre d'attributs
 CImg<> attributs(img.width(),img.height(),p);

 // Calcul de la moyenne et de la variance locale
 CImg<> N(9,9); 

 cimg_for9x9(img,x,y,0,0,N,float)
 {
  attributs(x,y,0) = N.mean();
  attributs(x,y,1) = N.variance();
 }

 return attributs;
}

/*******************************************************************************

                   Calcul de la distance au carrée

*******************************************************************************/
float d2(CImg<> attributs, int x, int y, float centMean, float centVariance)
{
 float d = 0.0;

 d = sqrt(pow(attributs(x,y,0)-centMean,2)  + pow(attributs(x,y,1)-centVariance,2));
 
 return d;
}



/*******************************************************************************

                        Fonction de classification

*******************************************************************************/
CImg<unsigned char> Kmeans(CImg<> attributs,int ncl){
 bool changed = true;
 float centroids[ncl][2];
 

 for (int i = 0; i < ncl; ++i){
 	centroids[i][0] = 0; 
 	centroids[i][1] = 0; 		
 }


 CImg<unsigned char> res(attributs.width(),attributs.height(),1,1,0);

 while(changed){
 	changed=false;
 	
 	std::cout<< "running" << std::endl;
 	cimg_forXY(attributs,x,y){
	 	int bestCent     = 0;
	 	int bestDistance = 256;

	 	for (int i = 0; i < ncl; ++i){
	 		
	 		int auxDist = d2(attributs,x,y,centroids[i][0],centroids[i][1]);
	 		if(auxDist < bestDistance){
	 			bestDistance = auxDist;
	 			bestCent=i;
	 		}
	 	}
	 	if(res(x,y)!=bestCent){
	 		res(x,y)=bestCent;
	 		
	 	}
 	}
 	
 	//Update the new centroid position

 		CImg<> buffer(ncl,2);
 		int nElements[ncl];
 		for (int i = 0; i < ncl; ++i)
 		{
 			buffer(i,0)=0;
 			buffer(i,1)=0;
 			nElements[i]=0;
 		}
 	cimg_forXY(res,x,y){
 		buffer(res(x,y),0) += attributs(x,y,0);
 		buffer(res(x,y),1) += attributs(x,y,1);
 		nElements[res(x,y)]+=1;
 	}

 	for (int i = 0; i < ncl; ++i)
 		{	
 			if(nElements[i]!=0){

 			float aux=buffer(i,0) / nElements[i];

 			if(aux!=centroids[i][0]){
 				centroids[i][0] = aux;
 				changed=true;
 			}

 			int aux=buffer(i,1) / nElements[i];

 			if(aux!=centroids[i][1]){
 				centroids[i][1] = aux;
 				changed=true;
 			}

 			}
 		}

 }

	cimg_forXY(res,x,y){
		res(x,y,0) = centroids[res(x,y)];

	}
 // Code

 return res;
}

/*******************************************************************************

                                      main

*******************************************************************************/
int main()
{

 CImg<> img = CImg<>("../Images/fish.bmp").channel(0);

 //img.resize(-50,-50);

 int ncl = 2;    // Nombre de classes
 CImg<>               attributs = AttributsExtraction(img);
 CImg<unsigned char>  etiquette = Kmeans(attributs,ncl);

 // Affichage finale
 CImgDisplay dispImg(img,"Image originale");
 CImgDisplay dispRes(etiquette.normalize(0,255),"Image segmentée");

 while (!dispImg.is_closed())
 {
  dispImg.wait();
 }
 return 0;
}

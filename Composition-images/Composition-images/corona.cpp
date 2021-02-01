#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"
#include <windows.h>
#include <tuple>
#include<vector>
#include <numeric>
#include "corona.hpp"

int main(int argc, char* argv[])
{
	std::cout << std::endl << "---------------------" << std::endl << "Initialisation du Programme" << std::endl << "---------------------" << std::endl;
	Image* tabImage;
	std::list<std::string> files;
	std::string fading = "opaque";
	int overlaps, distance;
	std::string jpeg = argv[argc -1];
	getParams(argc,argv,files,fading,overlaps,distance);
	int nbFichiers = files.size();



	std::cout << "Parametres recuperes, Nombre de fichiers a traiter : " << nbFichiers << std::endl << "Option Fading : "<< fading <<std::endl;
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	int width = tabImage[0].getWidth(); //Largeur
	int height = tabImage[0].getHeight(); //Hauteur



	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets en cours" << std::endl << "---------------------" << std::endl;
	Pixels** Mediane = median_images(tabImage, nbFichiers,width,height); //Application de la mediane
	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height);



	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets Termine" << std::endl << "---------------------" << std::endl;
	Image MedianeImg(width, height);
	MedianeImg.setTabPixels(Mediane);
	MedianeImg.setName("../Photos/Mediane.jpg");
	MedianeImg.saveImg();
	Image MedianeWithBlurImg(width, height);
	MedianeWithBlurImg.setTabPixels(MedianewithBlur);
	MedianeWithBlurImg.setName("../Photos/MedianeWithBlur.jpg");
	MedianeWithBlurImg.saveImg();



	std::cout << "---------------------" << std::endl << "Extraction des sujets et integration sur l'image de fond" << std::endl << "---------------------" << std::endl;
	Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height);



	//std::cout << "---------------------" << std::endl << "Application des filtres pour le rendu final" << std::endl << "---------------------" << std::endl;
	////fading
	//if (fading == "opaque") {
	//	Image MasqueAppliquer(width,height, jpeg);
	//	MasqueAppliquer.setTabPixels(MedianeAndMasque);
	//	MasqueAppliquer.saveImg();
	//}
	//if (fading == "plus") {
	//	Image fading_jpeg_front(width, height, jpeg);
	//	fading_jpeg_front.setTabPixels(Fading_front(tabImage, Mediane, nbFichiers, width, height));
	//	fading_jpeg_front.saveImg();
	//}
	//if (fading == "moins") {
	//	Image fading_jpeg_back(width, height, jpeg);
	//	fading_jpeg_back.setTabPixels(Fading_back(tabImage, Mediane, nbFichiers, width, height));
	//	fading_jpeg_back.saveImg();
	//}


	std::cout << "---------------------" << std::endl << "Overleap" << std::endl << "---------------------" << std::endl;
	Image overleapJPG(width, height, jpeg);
	Pixels** overleapTab = overlap(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height, 2);
	overleapJPG.setTabPixels(overleapTab);
	overleapJPG.saveImg();

	std::cout << "---------------------" << std::endl << "Programme termine" << std::endl << "---------------------" << std::endl;



}
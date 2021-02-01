#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"
#include <windows.h>
#include <tuple>
#include<vector>
#include <numeric>
#include "corona.hpp"





//void Connexe() {
//	corona::Image* masqueImg = corona::OpenImage("../Photos/Masque2.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
//	Pixels** masque = ImageToPixels(masqueImg);
//
//	int width = masqueImg->getWidth();
//	int height = masqueImg->getHeight();
//
//	Pixels** connexMax = init(width, height);
//
//	//int taille = tailleConnexe(masque, connexMax, height, width, 317, 61);
//	connexMax = plusGrandConnexe(masque, height, width);
//
//	TabToPixels(connexMax, masqueImg);
//	corona::SaveImage("../Photos/MasqueWithConnexe.jpg", corona::FileFormat::FF_PNG, masqueImg);
//
//
//}

int main(int argc, char* argv[])
{
	std::cout << std::endl << "---------------------" << std::endl << "Initialisation du Programme" << std::endl << "---------------------" << std::endl;
	Image* tabImage;
	std::list<std::string> files;
	std::string fading = "opaque";
	int overlaps, distance;
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
	MedianeImg.setTabPixels(MedianewithBlur);
	MedianeImg.setName("../Photos/Mediane.jpg");
	MedianeImg.saveImg();
	Image MedianeWithBlurImg(width, height);
	MedianeWithBlurImg.setTabPixels(MedianewithBlur);
	MedianeWithBlurImg.setName("../Photos/MedianeWithBlur.jpg");
	MedianeWithBlurImg.saveImg();



	//std::cout << "---------------------" << std::endl << "Extraction des sujets et integration sur l'image de fond" << std::endl << "---------------------" << std::endl;
	//Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height);



	//std::cout << "---------------------" << std::endl << "Application des filtres pour le rendu final" << std::endl << "---------------------" << std::endl;
	////fading
	//if (fading == "opaque") {
	//	Image MasqueAppliquer(width,height, "../Photos/MasqueAppliquer.jpg");
	//	MasqueAppliquer.setTabPixels(MedianeAndMasque);
	//	MasqueAppliquer.saveImg();
	//}
	//if (fading == "plus") {
	//	Image fading_jpeg_front(width, height, "../Photos/fading_image_front.jpg");
	//	fading_jpeg_front.setTabPixels(Fading_front(tabImage, Mediane, nbFichiers, width, height));
	//	fading_jpeg_front.saveImg();
	//}
	//if (fading == "moins") {


	//	Image fading_jpeg_back(width, height, "../Photos/fading_image_back.jpg");
	//	fading_jpeg_back.setTabPixels(Fading_back(tabImage, Mediane, nbFichiers, width, height));
	//	fading_jpeg_back.saveImg();
	//	
	//}


	std::cout << "---------------------" << std::endl << "Overleap" << std::endl << "---------------------" << std::endl;
	Image overleapJPG(width, height, "../Photos/overleapJPG.jpg");
	Pixels** overleapTab = overlap(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height, 2);
	overleapJPG.setTabPixels(overleapTab);
	overleapJPG.saveImg();

	std::cout << "---------------------" << std::endl << "Programme termine" << std::endl << "---------------------" << std::endl;



}
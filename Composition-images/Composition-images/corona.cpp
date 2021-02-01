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
	std::string fading = "";
	int overlaps = -1, distance = -1;
	bool parDefaut = true;
	std::string jpeg = argv[argc - 1];
	getParams(argc, argv, files, fading, overlaps, distance, parDefaut);
	int nbFichiers = files.size();



	std::cout << "Parametres recuperes, Nombre de fichiers a traiter : " << nbFichiers << std::endl << "Option Fading : " << fading << std::endl << "Option Overlap : " << overlaps << std::endl << "Option Distance : " << distance << std::endl;
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	int width = tabImage[0].getWidth(); //Largeur
	int height = tabImage[0].getHeight(); //Hauteur



	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets en cours" << std::endl << "---------------------" << std::endl;
	Pixels** Mediane = median_images(tabImage, nbFichiers,width,height); //Application de la mediane
	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height); // Mediane avec le flou Gaussien applique



	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets Termine" << std::endl << "---------------------" << std::endl;
	Image MedianeImg(width, height);
	MedianeImg.setTabPixels(Mediane);
	MedianeImg.setName("../Photos/Mediane.jpg");
	MedianeImg.saveImg();
	Image MedianeWithBlurImg(width, height);
	MedianeWithBlurImg.setTabPixels(MedianewithBlur);
	MedianeWithBlurImg.setName("../Photos/MedianeWithBlur.jpg");
	MedianeWithBlurImg.saveImg();

	std::cout << "---------------------" << std::endl << "Extraction des sujets" << std::endl << "---------------------" << std::endl;
	

	std::cout << "---------------------" << std::endl << "Application des filtres pour le rendu final" << std::endl << "---------------------" << std::endl;
	//fading
	if (fading == "opaque" ) {
		std::cout << "---------------------" << std::endl << "Application du filtre opaque" << std::endl << "---------------------" << std::endl;
		Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height);
		Image MasqueAppliquer(width,height, jpeg);
		MasqueAppliquer.setTabPixels(MedianeAndMasque);
		MasqueAppliquer.saveImg();
	}
	if (fading == "plus") {
		std::cout << "---------------------" << std::endl << "Application du filtre plus" << std::endl << "---------------------" << std::endl;
		Image fading_jpeg_front(width, height, jpeg);
		fading_jpeg_front.setTabPixels(Fading_front(tabImage, Mediane, nbFichiers, width, height));
		fading_jpeg_front.saveImg();
	}
	if (fading == "moins") {

		std::cout << "---------------------" << std::endl << "Application du filtre moins" << std::endl << "---------------------" << std::endl;
		Image fading_jpeg_back(width, height, jpeg);
		fading_jpeg_back.setTabPixels(Fading_back(tabImage, Mediane, nbFichiers, width, height));
		fading_jpeg_back.saveImg();
	}
	if (overlaps > -1) {
		std::cout << "---------------------" << std::endl << "Application du Overlap" << std::endl << "---------------------" << std::endl;
		Image overleapJPG(width, height, jpeg);
		Pixels** overleapTab = overlap(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height, overlaps);
		overleapJPG.setTabPixels(overleapTab);
		overleapJPG.saveImg();
	}
	if (distance != -1) {
		std::cout << "---------------------" << std::endl << "Application de la distance" << std::endl << "---------------------" << std::endl;
		Image distances(width, height, jpeg);
		distances.setTabPixels(overlap(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height, 1));
		distances.saveImg();
	}
	if (parDefaut == true) {
		std::cout << "---------------------" << std::endl << "Insertion des masques par defaut" << std::endl << "---------------------" << std::endl;
		Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height);
		Image MasqueAppliquer(width, height, jpeg);
		MasqueAppliquer.setTabPixels(MedianeAndMasque);
		MasqueAppliquer.saveImg();
	}
	std::cout << "---------------------" << std::endl << "Integration sur l'image de fond terminer" << std::endl << "---------------------" << std::endl;
	
	std::cout << "---------------------" << std::endl << "Programme termine" << std::endl << "---------------------" << std::endl;

	std::cout << "---------------------" << std::endl << "Made by Cedric Chopin / Guillaume Trem / Etienne Gibiat" << std::endl << "---------------------" << std::endl;

}
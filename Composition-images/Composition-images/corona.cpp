#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"
#include "corona.hpp"

char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	char** test = std::find(begin, end, option);
	return test != end;
}

int main(int argc, char* argv[])
{
	std::cout << std::endl << "---------------------" << std::endl << "Initialisation du Programme" << std::endl << "---------------------" << std::endl;
	Image* tabImage;
	std::list<Pixels**> tabPixels;
	std::list<std::string> files;
	std::string fading = "opaque";
	getParams(argc,argv,files,fading);
	int nbFichiers = files.size();
	std::cout << "Parametres recuperes, Nombre de fichiers a traiter : " << nbFichiers << std::endl << "Option Fading : "<< fading <<std::endl;
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	int width = tabImage[0].getWidth(); //Largeur
	int height = tabImage[0].getHeight(); //Hauteur
	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets en cours" << std::endl << "---------------------" << std::endl;
	Pixels** Mediane = median_images(tabImage, nbFichiers,width,height); //Application de la mediane
	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height);
	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets Termine" << std::endl << "---------------------" << std::endl;
	Image* MedianeImg = new Image(width, height);
	MedianeImg->setTabPixels(MedianewithBlur);
	MedianeImg->setName("../Photos/MedianeWithBlur.jpg");
	MedianeImg->saveImg();
	//Mediane = FlouGaussien(Mediane, width, height);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	

	//corona::Image* MedianeImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
	//Pixels** MedianewithBlur = ImageToPixels(MedianeImg);
	//Pixels** masque = CreationMasque(MedianewithBlur, tabPixels.front(), width, height);

	std::cout << "---------------------" << std::endl << "Extraction des sujets et integration sur l'image de fond" << std::endl << "---------------------" << std::endl;
	Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur, tabImage, nbFichiers, Mediane, width, height);

	std::cout << "---------------------" << std::endl << "Application des filtres pour le rendu final" << std::endl << "---------------------" << std::endl;

	//fading
	if (fading == "opaque") {
		Image* MasqueAppliquer = new Image(width,height, "../Photos/MasqueAppliquer.jpg");
		MasqueAppliquer->setTabPixels(MedianeAndMasque);
		MasqueAppliquer->saveImg();
	}
	if (fading == "plus") {
		Image* fading_jpeg_front = new Image(width, height, "../Photos/fading_image_front.jpg");
		Pixels** fading_image_front = Fading_front(tabImage, Mediane, nbFichiers,width, height);
		fading_jpeg_front->setTabPixels(fading_image_front);
		fading_jpeg_front->saveImg();
	}
	if (fading == "moins") {


		Image* fading_jpeg_back = new Image(width, height, "../Photos/fading_image_back.jpg");
		Pixels** fading_image_back = Fading_back(tabImage, Mediane,nbFichiers, width, height);
		fading_jpeg_back->setTabPixels(fading_image_back);
		fading_jpeg_back->saveImg();
		
	}
	std::cout << "---------------------" << std::endl << "Programme termine" << std::endl << "---------------------" << std::endl;

}
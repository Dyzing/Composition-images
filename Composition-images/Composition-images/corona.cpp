#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"


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
	corona::Image** tabImage;
	std::list<Pixels**> tabPixels;
	std::list<std::string> files;
	std::string fading = "opaque";
	getParams(argc,argv,files,fading);
	std::cout << "Parametres recuperes, Nombre de fichiers a traiter : " << files.size() << std::endl << "Option Fading : "<< fading <<std::endl;
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	tabPixels = initTabPixels(files.size() , tabImage); //Liste comportant les tableaux correspondants aux images
	int width = tabImage[0]->getWidth(); //Largeur
	int height = tabImage[0]->getHeight(); //Hauteur
	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets en cours" << std::endl << "---------------------" << std::endl;
	Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la mediane
	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height);
	std::cout << "---------------------" << std::endl << "Creation de l'image de fond sans les sujets Termine" << std::endl << "---------------------" << std::endl;
	corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianewithBlur, MedianeImg);
	corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);
	//Mediane = FlouGaussien(Mediane, width, height);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	

	//corona::Image* MedianeImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
	//Pixels** MedianewithBlur = ImageToPixels(MedianeImg);
	//Pixels** masque = CreationMasque(MedianewithBlur, tabPixels.front(), width, height);

	std::cout << "---------------------" << std::endl << "Extraction des sujets et integration sur l'image de fond" << std::endl << "---------------------" << std::endl;
	Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur,tabPixels, Mediane, width, height);

	std::cout << "---------------------" << std::endl << "Application des filtres pour le rendu final" << std::endl << "---------------------" << std::endl;
	//a effacer
	if (fading == "opaque") {
		corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
		TabToPixels(MedianeAndMasque, MasqueAppliquer);
		corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);
	}
	if (fading == "plus") {
		corona::Image* fading_jpeg_front = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
		Pixels** fading_image_front = Fading_front(tabPixels, Mediane, width, height);
		TabToPixels(fading_image_front, fading_jpeg_front);
		corona::SaveImage("../Photos/fading_image_front.jpg", corona::FileFormat::FF_PNG, fading_jpeg_front);
	}
	if (fading == "moins") {
		corona::Image* fading_jpeg_back = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
		Pixels** fading_image_back = Fading_back(tabPixels, Mediane, width, height);
		TabToPixels(fading_image_back, fading_jpeg_back);
		corona::SaveImage("../Photos/fading_image_back.jpg", corona::FileFormat::FF_PNG, fading_jpeg_back);
		
	}
	std::cout << "---------------------" << std::endl << "Programme terminer" << std::endl << "---------------------" << std::endl;


	corona::Image* connexe_jpeg = corona::OpenImage("../Photos/Masque2.jpg", corona::PF_R8G8B8A8);
	Pixels** tab_connexe_jpeg = ImageToPixels(connexe_jpeg);
	int cc_size_number = cc_size(tab_connexe_jpeg, connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), 455, 348);

}
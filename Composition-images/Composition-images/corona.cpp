#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"
#include <windows.h>
#include <tuple>
#include<vector>
#include <numeric>


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
	return std::find(begin, end, option) != end;
}

void CreateMedianes(std::list<Pixels**> tabPixels, corona::Image** tabImage) {

	int width = tabImage[0]->getWidth(); 
	int height = tabImage[0]->getHeight();

	Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight());

	corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(Mediane, MedianeImg);
	corona::SaveImage("../Photos/Mediane.jpg", corona::FileFormat::FF_PNG, MedianeImg);

	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height);
	
	TabToPixels(MedianewithBlur, MedianeImg);
	corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);

	delete Mediane;
	delete MedianeImg;
	delete MedianewithBlur;
}



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
	corona::Image** tabImage;
	std::list<Pixels**> tabPixels;
	std::list<std::string> files;

	//testConnexe();

	if (cmdOptionExists(argv, argv + argc, "-dir"))
	{
		char* filename = getCmdOption(argv, argv + argc, "-dir");
		files = FindFilesInDirectory(filename);
		std::cout << files.front();

	}
	else {
		for (int i = 1; i < argc /*- 1*/; i++) {
			files.push_back(argv[i]);
		}
	}
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	tabPixels = initTabPixels(files.size() , tabImage); //Liste comportant les tableaux correspondants aux images
	
	int width = tabImage[0]->getWidth(); //Largeur
	int height = tabImage[0]->getHeight(); //Hauteur

	Pixels** firstTab = tabPixels.front();

	
	
	//CreateMedianes(tabPixels, tabImage);

	//Mediane = FlouGaussien(Mediane, width, height);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	

	corona::Image* MedianewithBlurImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
	Pixels** MedianewithBlur = ImageToPixels(MedianewithBlurImg);
	corona::Image* MedianeImg = corona::OpenImage("../Photos/Mediane.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
	Pixels** Mediane = ImageToPixels(MedianeImg);
	//Pixels** masque = CreationMasque(MedianewithBlur, tabPixels.front(), width, height);

	Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur,tabPixels, Mediane, width, height);
	//Pixels** MedianeAndMasque = AppliquerMasque(Mediane, firstTab, Masque, width, height);
	//MedianeAndMasque = FlouGaussien(Mediane, width, height);
	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianeAndMasque, MasqueAppliquer);
	corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);

	//a effacer
	//corona::Image* fading_jpeg_front = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//Pixels** fading_image_front = Fading_front(tabPixels, Mediane, width, height );
	//TabToPixels(fading_image_front, fading_jpeg_front);
	//corona::SaveImage("../Photos/fading_image_front.jpg", corona::FileFormat::FF_PNG, fading_jpeg_front);

	//corona::Image* fading_jpeg_back = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//Pixels** fading_image_back = Fading_back(tabPixels, Mediane, width, height );
	//TabToPixels(fading_image_back, fading_jpeg_back);
	//corona::SaveImage("../Photos/fading_image_back.jpg", corona::FileFormat::FF_PNG, fading_jpeg_back);

}
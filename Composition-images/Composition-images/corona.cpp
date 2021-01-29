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
	return std::find(begin, end, option) != end;
}
int main(int argc, char* argv[])
{
	corona::Image** tabImage;
	std::list<Pixels**> tabPixels;
	std::list<std::string> files;
	if (cmdOptionExists(argv, argv + argc, "-dir"))
	{
		char* filename = getCmdOption(argv, argv + argc, "-dir");
		files = FindFilesInDirectory(filename);
		std::cout << files.front();

	}
	else {
		for (int i = 1; i < argc - 1; i++) {
			files.push_back(argv[i]);
		}
	}
	tabImage = initImage(files); // Tableau comportant la liste d'images passe en parametres
	tabPixels = initTabPixels(files.size() , tabImage); //Liste comportant les tableaux correspondants aux images
	
	int width = tabImage[0]->getWidth(); //Largeur
	int height = tabImage[0]->getHeight(); //Hauteur

	Pixels** firstTab = tabPixels.front();
	Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la mediane
	Pixels** MedianewithBlur = FlouGaussien(Mediane, width, height);
	corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianewithBlur, MedianeImg);
	corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);
	//Mediane = FlouGaussien(Mediane, width, height);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	

	//corona::Image* MedianeImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image mediane est deja creee, gain de temps
	//Pixels** MedianewithBlur = ImageToPixels(MedianeImg);
	//Pixels** masque = CreationMasque(MedianewithBlur, tabPixels.front(), width, height);

	Pixels** MedianeAndMasque = MultiMasque(MedianewithBlur,tabPixels, Mediane, width, height);
	//Pixels** MedianeAndMasque = AppliquerMasque(Mediane, firstTab, Masque, width, height);
	//MedianeAndMasque = FlouGaussien(Mediane, width, height);
	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianeAndMasque, MasqueAppliquer);
	corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);

	//a effacer
	corona::Image* fading_jpeg_front = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	Pixels** fading_image_front = Fading_front(tabPixels, Mediane, width, height );
	TabToPixels(fading_image_front, fading_jpeg_front);
	corona::SaveImage("../Photos/fading_image_front.jpg", corona::FileFormat::FF_PNG, fading_jpeg_front);

	corona::Image* fading_jpeg_back = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	Pixels** fading_image_back = Fading_back(tabPixels, Mediane, width, height );
	TabToPixels(fading_image_back, fading_jpeg_back);
	corona::SaveImage("../Photos/fading_image_back.jpg", corona::FileFormat::FF_PNG, fading_jpeg_back);

}
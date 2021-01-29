#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include "Masques.hpp"


int main(int argc, char* argv[])
{
	corona::Image** tabImage = initImage(argc - 1, argv); // Tableau comportant la liste d'images passe en parametres
	std::list<Pixels**> tabPixels = initTabPixels(argc - 1, tabImage); //Liste comportant les tableaux correspondants aux images

	int width = tabImage[0]->getWidth(); //Largeur
	int height = tabImage[0]->getHeight(); //Hauteur

	//Pixels** firstTab = tabPixels.front();
	//Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la mediane
	//Mediane = FlouGaussien(Mediane, width, height);
	
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	//corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);

	//Mediane = FlouGaussien(Mediane, width, height);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	corona::Image* MedianeImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image m�diane est deja creee, gain de temps
	Pixels** Mediane = ImageToPixels(MedianeImg);

	Pixels** MedianeAndMasque = MultiMasque(Mediane, tabPixels, width, height);

	//Pixels** MedianeAndMasque = AppliquerMasque(Mediane, firstTab, Masque, width, height);
	//MedianeAndMasque = FlouGaussien(Mediane, width, height);
	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianeAndMasque, MasqueAppliquer);
	corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);


}
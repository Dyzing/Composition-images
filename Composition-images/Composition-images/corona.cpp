#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <cmath>
#include<vector>
#include <algorithm>
/// <summary>
/// Structure d'un pixel en RGBA
/// </summary>
struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};
typedef unsigned char byte;
/// <summary>
/// Initialisation d'un tableau
/// </summary>
/// <param name="hauteur"></param>
/// <param name="largeur"></param>
/// <returns></returns>
Pixels** init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [hauteur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = &tab2[i * largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0,255 };
		}
	}

	return tab;
}
/// <summary>
/// Transfert le tableau de pixels dans l'image
/// </summary>
/// <param name="pix">Tableau de pixels</param>
/// <param name="img">Image qui recevra le tableau de pixels</param>
void TabToPixels(Pixels** pix, corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* p = img->getPixels();
	Pixels tmp;
	byte* iter = (byte*)p;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tmp = pix[i][j];
			*iter = tmp.red;
			*iter++;
			*iter = tmp.green;
			*iter++;
			*iter = tmp.blue;
			*iter++;
			*iter = tmp.alpha;
			*iter++;
		}
	}
}
/// <summary>
/// R�cup�ration de la liste d'image pass� en param�tres
/// </summary>
/// <param name="n">Nombre d'image</param>
/// <param name="arguments">chemin des images</param>
/// <returns>Tableau comportant toutes les images</returns>
corona::Image** initImage(int n, char* arguments[]) {
	corona::Image* img;
	corona::Image** tabImage = new corona::Image * [n];
	for (int i = 0; i < n; ++i) {
		img = corona::OpenImage(arguments[i+1], corona::PF_R8G8B8A8);
		tabImage[i] = img;
	}

	return tabImage;
}
/// <summary>
/// Convertit une image en tableau de pixels
/// </summary>
/// <param name="img">Image a convertir</param>
/// <returns>Tableau de pixels</returns>
Pixels** ImageToPixels(corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* pixels = img->getPixels();
	Pixels** tab = init(width, height);
	int red, green, blue, alpha;
	byte* p;
	// we're guaranteed that the first eight bits of every pixel is red,
	// the next eight bits is green, and so on...
	p = (byte*)pixels;
	for (int x = 0; x < height; ++x) {
		for (int j = 0; j < width; ++j) {
			red = *p++;
			green = *p++;
			blue = *p++;
			alpha = *p++;
			tab[x][j] = { red,green,blue,alpha };
		}
	}
	return tab;
}
/// <summary>
/// Initialisation des tableaux correspondant a la liste d'images
/// </summary>
/// <param name="taille">Nombre d'images</param>
/// <param name="tabImg">Liste d'images</param>
/// <returns>Liste des tableaux de pixels</returns>
std::list<Pixels**> initTabPixels(int taille, corona::Image** tabImg) {
	std::list<Pixels**> tabPixels;
	void* pixels;
	Pixels** tab;
	byte* p;
	int width, height, red, green, blue, alpha;
	for (int i = 0; i < taille; ++i) {
		tab = ImageToPixels(tabImg[i]);
		tabPixels.push_back(tab);
	}

	return tabPixels;
}

Pixels** test(Pixels** image, int width, int height, int radius) {

	Pixels** tab = init(width, height);

	for (int i = 0; i < width; i++) {

		for (int j = 0; j < height; j++) {

			
			tab[i][j].red = image[i][j].red;
			tab[i][j].green = image[i][j].green;
			tab[i][j].blue = image[i][j].blue;

			if (i == 25) {
				tab[i][j].red = 255;
				tab[i][j].green = 0;
				tab[i][j].blue = 0;
			}
			/*if (j == 26) {
				tab[i][j].red = 255;
				tab[i][j].green = 0;
				tab[i][j].blue = 0;
			}*/
		}
	}

	return tab;
}

Pixels** filtre_median(Pixels** image, int width, int height, int radius) {

	Pixels** tab = init(width, height);
	std::list<int> valR;
	std::list<int> valG;
	std::list<int> valB;
	int lenght;
	std::list<int>::iterator it;

	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			valR = {};
			valG = {};
			valB = {};
			
			for (int k = -radius; k <= radius; k++) {

				if (!(i + k > height - 1 || i + k < 0)) {

					for (int n = -radius; n <= radius; n++) {

						if (!(j + n > width - 1 || j + n < 0)) {

							valR.push_back(image[i + k][j + n].red);
							valG.push_back(image[i + k][j + n].green);
							valB.push_back(image[i + k][j + n].blue);

						}
					}
				}
			}

			valR.sort();
			valB.sort();
			valG.sort();

			lenght = valR.size();
			it = valR.begin();
			std::advance(it, lenght / 2);
			tab[i][j].red = *it;

			
			it = valG.begin();
			std::advance(it, lenght / 2);
			tab[i][j].green = *it;

			
			it = valB.begin();
			std::advance(it, lenght / 2);
			tab[i][j].blue = *it;
		}
	}

	return tab;
}

Pixels** median_images(std::list<Pixels**> images, int width, int height) {

	Pixels** tab = init(width, height);
	std::vector<int> valR;
	std::vector<int> valG;
	std::vector<int> valB;
	int n;
	Pixels p;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			n = 0;
			valR = {};
			valG = {};
			valB = {};
			std::list<int>::iterator it;
			for (Pixels** pixels : images) {
				n += 1;
				p = pixels[i][j];
				valR.push_back(p.red);
				valG.push_back(p.green);
				valB.push_back(p.blue);
			}
			std::sort(valR.begin(), valR.end());
			std::sort(valG.begin(), valG.end());
			std::sort(valB.begin(), valB.end());
			/*valR.sort();
			valG.sort();
			valB.sort();

			n /= 2;
			it = valR.begin();
			std::advance(it, n);
			tab[i][j].red = *it;
			it = valG.begin();
			std::advance(it, n);
			tab[i][j].green = *it;
			it = valB.begin();
			std::advance(it, n);
			tab[i][j].blue = *it;
			*/
			
			tab[i][j].red = valR[valR.size() / 2];
			tab[i][j].green = valR[valG.size() / 2];
			tab[i][j].blue = valR[valB.size() / 2];
			
		}
	}
	return tab;
}



struct PixelsFloat {

	float red;
	float green;
	float blue;
	float alpha;
};
Pixels** FlouGaussien(Pixels** img, int width, int height) {
	corona::Image* cop = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(img, cop);
	Pixels** pix = ImageToPixels(cop);
	int matConvo[5][5] = { {1, 4, 7, 4, 1},{4, 16, 26, 16, 4},{7, 26, 41, 26, 7},{4, 16, 26, 16, 4},{1, 4, 7, 4, 1} };
	PixelsFloat sumVois = { 0,0,0,255 };
	for (int x = 2; x < height - 2; x++) {
		for (int y = 2; y < width - 2; y++) {
			sumVois = { 0,0,0,255 };
			for (int xvois = x - 2; xvois < x + 3; xvois++) {
				if (xvois >= 0 and xvois < height) {
					for (int yvois = y - 2; yvois < y + 3; yvois++) {
						if (yvois >= 0 and yvois < width) {
							sumVois.red = sumVois.red + img[xvois][yvois].red * (matConvo[xvois - x + 2][yvois - y + 2] / 273.f);
							sumVois.green = sumVois.green + img[xvois][yvois].green * (matConvo[xvois - x + 2][yvois - y + 2] / 273.f);
							sumVois.blue = sumVois.blue + img[xvois][yvois].blue * (matConvo[xvois - x + 2][yvois - y + 2] / 273.f);

						}
					}
				}
			}
			pix[x][y] = { (int)sumVois.red,(int)sumVois.green ,(int)sumVois.blue ,(int)sumVois.alpha };

		}
	}

	return pix;
}

Pixels** flou_glaussien(corona::Image* img, int radius)
{
	corona::Image* image_floutee = corona::CloneImage(img);
	Pixels** tab2 = ImageToPixels(image_floutee);
	Pixels** tab = ImageToPixels(img);
	int height = img->getHeight();
	int width = img->getWidth();
	int borne_inf_x, borne_inf_y, borne_sup_x, borne_sup_y, iterateur;
	float somme_rgb_pond_red, somme_rgb_pond_green, somme_rgb_pond_blue;
	float tabconvo[] = { 1, 4, 7, 4, 1, 4, 16, 26, 16, 4, 7, 26, 41, 26, 7, 4, 16, 26, 16, 4, 1, 4, 7, 4, 1 };

	for (size_t i = 2; i < height - 2; i++)
	{
		for (size_t j = 2; j < width - 2; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > height)
				borne_sup_x = height;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > width)
				borne_sup_y = width;

			somme_rgb_pond_red = 0;
			somme_rgb_pond_green = 0;
			somme_rgb_pond_blue = 0;
			iterateur = 0;

			for (size_t x = borne_inf_x; x < borne_sup_x; x++)
			{
				for (size_t y = borne_inf_y; y < borne_sup_y; y++)
				{
					somme_rgb_pond_red += tabconvo[iterateur] / 273.0f * tab[x][y].red;
					somme_rgb_pond_green += tabconvo[iterateur] / 273.0f * tab[x][y].green;
					somme_rgb_pond_blue += tabconvo[iterateur] / 273.0f * tab[x][y].blue;
					++iterateur;
				}
			}
			tab2[i][j].red = (int)somme_rgb_pond_red;
			tab2[i][j].green = (int)somme_rgb_pond_green;
			tab2[i][j].blue = (int)somme_rgb_pond_blue;
		}
	}
	TabToPixels(tab2, image_floutee);
	corona::SaveImage("../Photos/image_floutee_gaussien.jpg", corona::FileFormat::FF_PNG, image_floutee);
	return tab2;
}


Pixels** flou_moyen(corona::Image* img, int radius)
{
	int height = img->getHeight();
	int width = img->getWidth();
	corona::Image* image_floutee_moyen = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	Pixels** tab2 = ImageToPixels(image_floutee_moyen); //pix2 = cop.load() 					 	 			    	  
	Pixels** tab = ImageToPixels(img); //pix1 = im.load() 					 	 			    	  
	int borne_inf_x, borne_inf_y, borne_sup_x, borne_sup_y, iterateur;
	float somme_rgb_pond_red, somme_rgb_pond_green, somme_rgb_pond_blue;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > height)
				borne_sup_x = height;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > width)
				borne_sup_y = width;

			somme_rgb_pond_red = 0;
			somme_rgb_pond_green = 0;
			somme_rgb_pond_blue = 0;
			iterateur = 0;

			for (int x = borne_inf_x; x < borne_sup_x; x++)
			{
				for (int y = borne_inf_y; y < borne_sup_y; y++)
				{
					somme_rgb_pond_red += tab[x][y].red;
					somme_rgb_pond_green += tab[x][y].green;
					somme_rgb_pond_blue += tab[x][y].blue;
					++iterateur;
				}
			}

			tab2[i][j].red = (int)somme_rgb_pond_red / iterateur;
			tab2[i][j].green = (int)somme_rgb_pond_green / iterateur;
			tab2[i][j].blue = (int)somme_rgb_pond_blue / iterateur;
			tab2[i][j].alpha = 255;
		}
	}
	TabToPixels(tab2, image_floutee_moyen);
	corona::SaveImage("../Photos/image_floutee_moyen.jpg", corona::FileFormat::FF_PNG, image_floutee_moyen);
	return tab2;
}


/// <summary>
/// Creation du masque correspondant au sujet de l'image
/// </summary>
/// <param name="Fond">Fond de l'image sans le sujet</param>
/// <param name="Image">Image où seras extrait le sujet</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Retourne le masque</returns>
Pixels** CreationMasque(Pixels** Fond, Pixels** Image, int width, int height) {
	corona::Image* temp = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(Image, temp);
	Image = flou_glaussien(temp, 2);
	//Image = FlouGaussien(Image, width, height);
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tabFinal[i][j].red = std::abs(Fond[i][j].red - Image[i][j].red);
			tabFinal[i][j].green = std::abs(Fond[i][j].green - Image[i][j].green);
			tabFinal[i][j].blue = std::abs(Fond[i][j].blue - Image[i][j].blue);
		}
	}

	return tabFinal;
}
/// <summary>
/// Permet l'application du masque à partir de l'image de base sur le Fond
/// </summary>
/// <param name="Fond">Fond de l'image sans le sujet</param>
/// <param name="ImgBase">Image de base comportant le fond et le sujet</param>
/// <param name="Masque">Masque correspondant au sujet sur l'image de base</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height) {
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (Masque[i][j].red > 18)
				tabFinal[i][j].red = ImgBase[i][j].red;
			else
				tabFinal[i][j].red = Fond[i][j].red;
			if (Masque[i][j].green > 18)
				tabFinal[i][j].green = ImgBase[i][j].green;
			else
				tabFinal[i][j].green = Fond[i][j].green;
			if (Masque[i][j].blue > 18)
				tabFinal[i][j].blue = ImgBase[i][j].blue;
			else
				tabFinal[i][j].blue = Fond[i][j].blue;
		}
	}
	return tabFinal;
}



Pixels** test(corona::Image* img, int radius)
{
	int height = img->getHeight();
	int width = img->getWidth();
	corona::Image* image_floutee_moyen = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	Pixels** tab2 = ImageToPixels(image_floutee_moyen); //pix2 = cop.load() 					 	 			    	  
	Pixels** tab = ImageToPixels(img); //pix1 = im.load() 					 	 			    	  
	int borne_inf_x, borne_inf_y, borne_sup_x, borne_sup_y, iterateur;
	float somme_rgb_pond_red, somme_rgb_pond_green, somme_rgb_pond_blue;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > height)
				borne_sup_x = height;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > width)
				borne_sup_y = width;

			somme_rgb_pond_red = 0;
			somme_rgb_pond_green = 0;
			somme_rgb_pond_blue = 0;
			iterateur = 0;

			for (int x = borne_inf_x; x < borne_sup_x; x++)
			{
				for (int y = borne_inf_y; y < borne_sup_y; y++)
				{
					somme_rgb_pond_red += tab[x][y].red;
					somme_rgb_pond_green += tab[x][y].green;
					somme_rgb_pond_blue += tab[x][y].blue;
					++iterateur;
				}
			}

			/*tab2[i][j].red = (int)somme_rgb_pond_red / iterateur;
			tab2[i][j].green = (int)somme_rgb_pond_green / iterateur;
			tab2[i][j].blue = (int)somme_rgb_pond_blue / iterateur;
			tab2[i][j].alpha = 255;*/

			tab2[i][j].red = tab[i][j].red;
			tab2[i][j].green = tab[i][j].green;
			tab2[i][j].blue = tab[i][j].blue;
			tab2[i][j].alpha = 255;

			if (i == 200)
			{
				tab2[i][j] = { 0,255,0,255 };
			}
		}
	}
	TabToPixels(tab2, image_floutee_moyen);
	corona::SaveImage("../Photos/image_test.jpg", corona::FileFormat::FF_PNG, image_floutee_moyen);
	return tab2;
}


/// <summary>
/// Fonction appliquant tout les masques correspondant aux images pass�s en param�tre
/// </summary>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="tabPixels">Liste des images sous formes de tableau de pixels</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>R�sultat final comportant le fond + les sujets</returns>
Pixels** MultiMasque(Pixels** Mediane, std::list<Pixels**> tabPixels, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = Mediane;
	Pixels** tmp;
	corona::Image* img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	for (Pixels** tab : tabPixels) {
		masque = CreationMasque(Mediane, tab, width, height);
		resFinal = AppliquerMasque(resFinal, tab, masque, width, height);
	}

	return resFinal;
}

int main(int argc, char* argv[])
{	
	//corona::Image* img = corona::OpenImage("../Photos/test.jpg", corona::PF_R8G8B8A8);
	//int width = img->getWidth(); //Largeur
	//int height = img->getHeight(); //Hauteur

	//Pixels** filtreMedian = filtre_median(ImageToPixels(img), width, height, 2);
	//corona::Image* imageFiltreMedian = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(filtreMedian, imageFiltreMedian);
	//corona::SaveImage("../Photos/test_filtremedian.jpg", corona::FileFormat::FF_PNG, imageFiltreMedian);

	//Pixels** firstTab = tabPixels.front();
	//Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la médiane
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	//corona::SaveImage("../Photos/Mediane.jpg", corona::FileFormat::FF_PNG, MedianeImg);

	////corona::Image* MedianeImg = corona::OpenImage("../Photos/MasqueAppliquer.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image médiane est déjà créée, gain de temps
	////Pixels** Mediane = ImageToPixels(MedianeImg);
	//Pixels** Masque = CreationMasque(Mediane, firstTab, width, height); //Creation du masque
	//AppliquerMasque(Mediane, firstTab, Masque, width, height); //Application du masque




	//corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MasqueAppliquer);

	//corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);



	/*Pixels** masque1 = CreationMasque(ImageToPixels(corona::OpenImage("../Photos/Mediane.jpg", corona::PF_R8G8B8A8)), ImageToPixels(corona::OpenImage("../Photos/chrono2/chrono21.png", corona::PF_R8G8B8A8)), width, height);
	corona::Image* imageMasque1 = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(masque1, imageMasque1);
	corona::SaveImage("../Photos/masque1.jpg", corona::FileFormat::FF_PNG, imageMasque1);*/
	

	corona::Image** tabImage = initImage(argc-1, argv); // Tableau comportant la liste d'images pass� en param�tres
	std::list<Pixels**> tabPixels = initTabPixels(argc-1, tabImage); //Liste comportant les tableaux correspondants aux images

	int width = tabImage[0]->getWidth(); //Largeur
	int height = tabImage[0]->getHeight(); //Hauteur

	Pixels** firstTab = tabPixels.front();
	Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la m�diane
	/*Mediane = FlouGaussien(Mediane, width, height);
	corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(Mediane, MedianeImg);
	corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);*/
	corona::Image* MedianeImg = corona::OpenImage("../Photos/MedianeWithBlur.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image m�diane est d�j� cr��e, gain de temps
	
	corona::Image* tempMediane = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(Mediane, tempMediane);

	Pixels** Mediane_flou_moyen = flou_glaussien(tempMediane, 2);

	//Pixels** Mediane = ImageToPixels(MedianeImg);
	//Mediane = FlouGaussien(Mediane, width, height);
	//Pixels** Masque = CreationMasque(Mediane_flou_moyen, first_image_flou_moyen, width, height); //Creation du masque
	//AppliquerMasque(Mediane, firstTab, Masque, width, height); //Application du masque
	
	Pixels** MedianeAndMasque = MultiMasque(Mediane_flou_moyen, tabPixels, width, height);

	//Pixels** MedianeAndMasque = AppliquerMasque(Mediane, firstTab, Masque, width, height);
	//MedianeAndMasque = FlouGaussien(Mediane, width, height);
	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(MedianeAndMasque, MasqueAppliquer);
	
	corona::Image* image = tabImage[3];
	
	//Pixels** tab = tabPixels.back();

	Pixels** image_flou_gaussien = flou_glaussien(tabImage[0], 2);

	//Pixels** Mediane = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight()); //Application de la m�diane
	//Mediane = FlouGaussien(Mediane, width, height);
	//Mediane = filtre_median(Mediane, width, height, 2);
	//corona::Image* MedianeImg = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Mediane, MedianeImg);
	//corona::SaveImage("../Photos/MedianeWithBlur.jpg", corona::FileFormat::FF_PNG, MedianeImg);
	//corona::Image* MedianeImg = corona::OpenImage("../Photos/Mediane.jpg", corona::PF_R8G8B8A8); // Retirer le commentaire si l'image m�diane est d�j� cr��e, gain de temps
	//Pixels**  Mediane = ImageToPixels(MedianeImg);
	////Mediane = FlouGaussien(Mediane, width, height);
	//Mediane = filtre_median(Mediane, width, height, 2);
	//firstTab = filtre_median(firstTab, width, height, 2);
	//Pixels** Masque = CreationMasque(Mediane, firstTab, width, height); //Creation du masque
	//corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	//TabToPixels(Masque, MasqueAppliquer);
	//corona::SaveImage("../Photos/Masque1.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);
	//AppliquerMasque(Mediane, firstTab, Masque, width, height); //Application du masque
	//
	//Pixels** MedianeAndMasque = MultiMasque(Mediane, tabPixels, width, height);

	//MedianeAndMasque = AppliquerMasque(Mediane, firstTab, Masque, width, height);
	//MedianeAndMasque = FlouGaussien(Mediane, width, height);
	//
	//TabToPixels(Masque, MasqueAppliquer);

	corona::SaveImage("../Photos/MasqueAppliquer.jpg", corona::FileFormat::FF_PNG, MasqueAppliquer);


}
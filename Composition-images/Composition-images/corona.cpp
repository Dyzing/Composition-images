#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <cmath>
struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};
typedef unsigned char byte;

Pixels** init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [largeur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < largeur; i++) {
		tab[i] = &tab2[i * hauteur];

		for (int j = 0; j < hauteur; j++) {
			tab[i][j] = { 0,0,255,255 };
		}
	}

	return tab;
}

void TabToPixels(Pixels** pix, corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* p = img->getPixels();
	Pixels tmp;
	byte* iter = (byte*)p;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
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

corona::Image** initImage(int n, char* arguments[]) {
	corona::Image* img;
	corona::Image** tabImage = new corona::Image * [n];
	for (int i = 0; i < n; ++i) {
		img = corona::OpenImage(arguments[i+1], corona::PF_R8G8B8A8);
		tabImage[i] = img;
	}

	return tabImage;
}
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
	for (int x = 0; x < width; ++x) {
		for (int j = 0; j < height; ++j) {
			red = *p++;
			green = *p++;
			blue = *p++;
			alpha = *p++;
			tab[x][j] = { red,green,blue,alpha };
		}
	}
	return tab;
}
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

Pixels** median_images(std::list<Pixels**> images, int width, int height) {

	Pixels** tab = init(width, height);
	std::list<int> valR;
	std::list<int> valG;
	std::list<int> valB;
	int n;
	Pixels p;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
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
			valR.sort();
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
		}
	}
	return tab;
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

	for (size_t i = 2; i < width - 2; i++)
	{
		for (size_t j = 2; j < height - 2; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > width)
				borne_sup_x = width;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > height)
				borne_sup_y = height;

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
	corona::SaveImage("../Photos/image_floutee.jpg", corona::FileFormat::FF_PNG, image_floutee);
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

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > width)
				borne_sup_x = width;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > height)
				borne_sup_y = height;

			somme_rgb_pond_red = 0;
			somme_rgb_pond_green = 0;
			somme_rgb_pond_blue = 0;
			iterateur = 0;

			for (int x = borne_inf_x; x < borne_sup_x; x++)
			{
				for (int y = borne_inf_y; y < borne_sup_y; y++)
				{
					somme_rgb_pond_red += tab[x][y].red;
					somme_rgb_pond_green +=  tab[x][y].green;
					somme_rgb_pond_blue +=  tab[x][y].blue;
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


Pixels** test(corona::Image* img, int radius)
{
	int height = img->getHeight();
	int width = img->getWidth();
	corona::Image* image_floutee_moyen = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	Pixels** tab2 = ImageToPixels(image_floutee_moyen); //pix2 = cop.load() 					 	 			    	  
	Pixels** tab = ImageToPixels(img); //pix1 = im.load() 					 	 			    	  
	int borne_inf_x, borne_inf_y, borne_sup_x, borne_sup_y, iterateur;
	float somme_rgb_pond_red, somme_rgb_pond_green, somme_rgb_pond_blue;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			borne_inf_x = i - radius;
			borne_sup_x = i + radius + 1;
			borne_inf_y = j - radius;
			borne_sup_y = j + radius + 1;

			if (borne_inf_x < 0)
				borne_inf_x = 0;
			if (borne_sup_x > width)
				borne_sup_x = width;
			if (borne_inf_y < 0)
				borne_inf_y = 0;
			if (borne_sup_y > height)
				borne_sup_y = height;

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





int main(int argc, char* argv[])
{	
	corona::Image** tabImage = initImage(argc-1, argv);
	std::list<Pixels**> tabPixels = initTabPixels(argc-1, tabImage);

	int width = tabImage[0]->getWidth();
	int height = tabImage[0]->getHeight();

	Pixels** tab1 = init(width, height);
	Pixels** tab2 = init(width, height);

	//Pixels** tab = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight());

	//Pixels**  tab_flou = flou_glaussien(tabImage[0], 2);
	Pixels** tab_flou_moyen = flou_moyen(tabImage[0], 2);
	Pixels** testab = test(tabImage[0], 2);

	corona::Image* imagecop2 = corona::CloneImage(tabImage[0]);
	TabToPixels(tab2, imagecop2);

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (j == height / 2)
			{
				tab1[i][j] = { 255,0,0,255 };
			}
		}
	}

	corona::Image* imagecop1 = corona::CloneImage(tabImage[0]);
	TabToPixels(tab1, imagecop1);

	Pixels** tab3 = init(width, height);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			tab3[i][j].red = std::abs(tab1[i][j].red - testab[i][j].red);
			tab3[i][j].green = std::abs(tab1[i][j].green - testab[i][j].green);
			tab3[i][j].blue = std::abs(tab1[i][j].blue - testab[i][j].blue);
			//tab3[i][j].alpha = tab2[i][j].alpha - tab1[i][j].alpha;
		}
	}

	corona::Image* imagecop3 = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(tab3, imagecop3);

	corona::SaveImage("../Photos/imagecop1.jpg", corona::FileFormat::FF_PNG, imagecop1);
	corona::SaveImage("../Photos/imagecop2.jpg", corona::FileFormat::FF_PNG, imagecop2);
	corona::SaveImage("../Photos/imagecop3.jpg", corona::FileFormat::FF_PNG, imagecop3);

	
	corona::Image* image = tabImage[3];
	
	//Pixels** tab = tabPixels.back();
	
	
	
	corona::Image*  imagecop =	corona::CloneImage(image);
	TabToPixels(tab1, imagecop);
	corona::SaveImage("../Photos/coucou.jpg", corona::FileFormat::FF_PNG, imagecop);

}
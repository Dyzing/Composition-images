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

Pixels** init(int hauteur, int largeur) {
	Pixels** tab = new Pixels * [hauteur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = &tab2[i * largeur];

		for (int j = 0; j < largeur; j++) {
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

std::list<Pixels**> initTabPixels(int taille, corona::Image** tabImg) {
	std::list<Pixels**> tabPixels;
	void* pixels;
	Pixels** tab;
	byte* p;
	int width, height, red, green, blue, alpha;
	for (int i = 0; i < taille; ++i) {
		width = tabImg[i]->getWidth();
		height = tabImg[i]->getHeight();
		pixels = tabImg[i]->getPixels();
		tab = init(width,height);
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


int main(int argc, char* argv[])
{	
	corona::Image** tabImage = initImage(argc-1, argv);
	std::list<Pixels**> tabPixels = initTabPixels(argc-1, tabImage);

	int width = tabImage[0]->getWidth();
	int height = tabImage[0]->getHeight();

	Pixels** tab1 = init(width, height);
	Pixels** tab2 = init(width, height);

	Pixels** tab = median_images(tabPixels, tabImage[0]->getWidth(), tabImage[0]->getHeight());



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
			tab3[i][j].red = std::abs(tab[i][j].red - tabPixels.front()[i][j].red);
			tab3[i][j].green = std::abs(tab[i][j].green - tabPixels.front()[i][j].green);
			tab3[i][j].blue = std::abs(tab[i][j].blue - tabPixels.front()[i][j].blue);
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
	TabToPixels(tab, imagecop);
	corona::SaveImage("../Photos/coucou.jpg", corona::FileFormat::FF_PNG, imagecop);

}
#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
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
			tab[i][j] = { 0,0,0,255 };
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
int main(int argc, char* argv[])
{	
	corona::Image** tabImage = initImage(argc-1, argv);
	std::list<Pixels**> tabPixels = initTabPixels(argc-1, tabImage);
	
	corona::Image* image = tabImage[3];
	
	Pixels** tab = tabPixels.back();
	
	
	
	corona::Image*  imagecop =	corona::CloneImage(image);
	TabToPixels(tab, imagecop);
	corona::SaveImage("../Photos/coucou.jpg", corona::FileFormat::FF_PNG, imagecop);

}
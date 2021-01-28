#include "Initialisation-Conversion.hpp"

Pixels * *init(int largeur, int hauteur) {
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

corona::Image** initImage(int n, char* arguments[]) {
	corona::Image* img;
	corona::Image** tabImage = new corona::Image * [n];
	for (int i = 0; i < n; ++i) {
		img = corona::OpenImage(arguments[i + 1], corona::PF_R8G8B8A8);
		tabImage[i] = img;
	}

	return tabImage;
}


std::list<Pixels**> initTabPixels(int taille, corona::Image** tabImg) {
	std::list<Pixels**> tabPixels;
	Pixels** tab;
	for (int i = 0; i < taille; ++i) {
		tab = ImageToPixels(tabImg[i]);
		tabPixels.push_back(tab);
	}

	return tabPixels;
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
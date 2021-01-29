#include "Filtres.hpp"
#include <algorithm>
#include <array>


Pixels * *FlouGaussien(Pixels * *img, int width, int height) {
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
	const int size = 50;
	std::list<int> valR;
	std::list<int> valG;
	std::list<int> valB;

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
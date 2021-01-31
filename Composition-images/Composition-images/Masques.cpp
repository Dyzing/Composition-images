#include "Masques.hpp"
#include<string>
#include <tuple>
#include<vector>
#include <numeric>
#include <stack>
#include <tuple>

Pixels** CreationMasque(Pixels** Fond, Pixels** Img, int width, int height, int n) {
	Img = FlouGaussien(Img, width, height);
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tabFinal[i][j].red = std::abs(Fond[i][j].red - Img[i][j].red);
			tabFinal[i][j].green = std::abs(Fond[i][j].green - Img[i][j].green);
			tabFinal[i][j].blue = std::abs(Fond[i][j].blue - Img[i][j].blue);
		}
	}
	tabFinal = plusGrandConnexe(tabFinal, height, width);

	Image MasqueAppliquer(width, height, "../Photos/Masque" + std::to_string(n) + ".jpg");
	MasqueAppliquer.setTabPixels(tabFinal);
	MasqueAppliquer.saveImg();
	//std::string test = std::to_string(n);
	//std::string test2 = "../Photos/Masque";
	//test2 += test + ".jpg";
	//char* cstr;
	//cstr = &test2[0];



	return tabFinal;
}
int tailleConnexe(Pixels** tab, Pixels** copyTab, int height, int width, int x, int y) {

	std::tuple<int, int> t;
	std::vector<std::tuple<int, int>> v;
	v.push_back(std::make_tuple(x, y));

	int count = 0;
	int i, j;
	Pixels p;
	while (!v.empty()) {
		t = v.back();
		v.pop_back();
		i = std::get<0>(t);
		j = std::get<1>(t);
		p = tab[i][j];
		if (!(p <= 2)) {
			count += 1;
			tab[i][j] = { 0,0,0};

			copyTab[i][j] = p;
			for (int n = -1; n < 2; n++) {
				if (!(i + n > height - 1 or i + n < 0)) {
					for (int k = -1; k < 2; k++) {
						if (!(j + k > width - 1 or j + k < 0)) {
								v.push_back(std::make_tuple(i + n, j + k));
						}
					}
				}
			}
		}
	}

	return count;
}
Pixels** plusGrandConnexe(Pixels** tab, int height, int width) {

	Pixels p;
	int taille;
	int max = 0;
	Pixels** connex = init(width, height);
	Pixels** connexMax = init(width, height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			p = tab[i][j];
			if (!(p.red <= 2 && p.green <= 2 && p.blue <= 2)) {
				for (int k = 0; k < height; k++) {
					for (int n = 0; n < width; n++) {
						connex[k][n] = { 0, 0, 0};
					}
				}
				taille = tailleConnexe(tab, connex, height, width, i, j);
				if (taille > max) {
					max = taille;
					for (int k = 0; k < height; k++)
					{
						for (int n = 0; n < width; n++) {
							connexMax[k][n] = connex[k][n];
						}
					}

				}
			}
		}
	}
	for (int i = 0; i < height; i++)
	{
			delete[] connex[i];
	}
	delete[] connex;
	return connexMax;
}



Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height) {

	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!(Masque[i][j] <= 12)) {
				tabFinal[i][j] = ImgBase[i][j];
			}
			else {
				tabFinal[i][j] = Fond[i][j];
			}

		}
	}
	return tabFinal;
}


Pixels** MultiMasque(Pixels** Mediane, Image* tabImage,int nb, Pixels** fond, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = fond;
	Pixels** pixels;
	for (int x = 0; x < nb; ++x) {
		pixels = tabImage[x].getTabPixels();
		masque = CreationMasque(Mediane, pixels, width, height, x);
		resFinal = AppliquerMasque(resFinal, pixels, masque, width, height);
		for (int i = 0; i < height; i++)
		{
			delete[] masque[i];
			delete[] pixels[i];
		}
		delete[] masque;
		delete[] pixels;
	}
	
	return resFinal;
}


Pixels** MoyenneNimages(std::list<Pixels**> tabPixels, int width, int height)
{

	Pixels** image_res = init(width, height);
	int max_r, max_g, max_b, sum_r, sum_g, sum_b;
	Pixels p;
	int N = tabPixels.size();
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			sum_r = 0;
			sum_g = 0;
			sum_b = 0;

			for (int i = 0; i < tabPixels.size(); i++)
			{
				Pixels** tmp = tabPixels.front();

				sum_r += tmp[x][y].red;
				sum_g += tmp[x][y].green;
				sum_b += tmp[x][y].blue;
				tabPixels.pop_front();
				for (int i = 0; i < height; i++) {
					delete[] tmp[i];
				}
				delete[] tmp;
			}


			max_r = int(sum_r / N);
			max_g = int(sum_g / N);
			max_b = int(sum_b / N);

			image_res[x][y] = { max_r, max_g, max_b};
		}
	}
	return image_res;
}

Pixels** Fading_front(Image* tabImage, Pixels** Mediane,int nb,int width, int height)
{ 
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tabPixels;
	std::list<Pixels**> tempPix;
	for (int x = 0; x < nb; ++x) {
		tabPixels.push_back(tabImage[x].getTabPixels());
	}
	tempPix.push_back(tabPixels.front());
	for (int x = 0; x < height; ++x) {
		delete []tabPixels.front()[x];
	}
	delete[]tabPixels.front();
	tabPixels.pop_front();
	int nbtabPixels = tabPixels.size();
	for (int i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.front());
		for (int x = 0; x < height; ++x) {
			delete[]tabPixels.front()[x];
		}
		delete[]tabPixels.front();
		tabPixels.pop_front();
		res_moy = MoyenneNimages(tempPix, width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.back(), width, height, 101);
	res_moy = AppliquerMasque(res_moy, tabPixels.back(), masque, width, height);
	for (int x = 0; x < height; ++x) {
		delete[]tempPix.front()[x];
	}
	delete[]tempPix.front();
	return res_moy;
}

Pixels** Fading_back(Image* tabImage, Pixels** Mediane,int nb ,int width, int height)
{

	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tabPixels;
	std::list<Pixels**> tempPix;
	for (int x = 0; x < nb; ++x) {
		tabPixels.push_back(tabImage[x].getTabPixels());
	}
	tempPix.push_back(tabPixels.back());
	tabPixels.pop_back();
	int nbtabPixels = tabPixels.size();
	for (int i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.back());
		tabPixels.pop_back();
		res_moy = MoyenneNimages(tempPix,width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.front(), width, height, 102);
	res_moy = AppliquerMasque(res_moy, tabPixels.front(), masque, width, height);

	return res_moy;
}






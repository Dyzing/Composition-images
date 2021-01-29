#include "Masques.hpp"
#include <vector>
#include <numeric>
#include<string>


Pixels * *CreationMasque(Pixels * *Fond, Pixels * *Image, int width, int height, int n) {
	Image = FlouGaussien(Image, width, height);
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tabFinal[i][j].red = std::abs(Fond[i][j].red - Image[i][j].red);
			tabFinal[i][j].green = std::abs(Fond[i][j].green - Image[i][j].green);
			tabFinal[i][j].blue = std::abs(Fond[i][j].blue - Image[i][j].blue);
		}
	}

	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(tabFinal, MasqueAppliquer);
	std::string test = std::to_string(n);
	std::string test2 = "../Photos/Masque";
	test2 += test + ".jpg";
	char* cstr;
	cstr = &test2[0];

	corona::SaveImage(cstr, corona::FileFormat::FF_PNG, MasqueAppliquer);

	return tabFinal;
}

Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height) {

	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!(Masque[i][j].red <= 12 && Masque[i][j].green <= 12 && Masque[i][j].blue <= 12)) {
				tabFinal[i][j].red = ImgBase[i][j].red;
				tabFinal[i][j].green = ImgBase[i][j].green;
				tabFinal[i][j].blue = ImgBase[i][j].blue;
			}
			else {
				tabFinal[i][j].red = Fond[i][j].red;
				tabFinal[i][j].green = Fond[i][j].green;
				tabFinal[i][j].blue = Fond[i][j].blue;
			}
			/*if (Masque[i][j].green > 15)
				tabFinal[i][j].green = ImgBase[i][j].green;
			else
				tabFinal[i][j].green = Fond[i][j].green;
			if (Masque[i][j].blue > 18)
				tabFinal[i][j].blue = ImgBase[i][j].blue;
			else
				tabFinal[i][j].blue = Fond[i][j].blue;*/

		}
	}
	return tabFinal;
}


Pixels** MultiMasque(Pixels** Mediane, std::list<Pixels**> tabPixels, Pixels** fond, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = fond;
	corona::Image* img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	int n = 0;
	for (Pixels** tab : tabPixels) {
		masque = CreationMasque(Mediane, tab, width, height, n);
		resFinal = AppliquerMasque(resFinal, tab, masque, width, height);
		n += 1;
	}

	return resFinal;
}


Pixels** MoyenneNimages(std::list<Pixels**> tabPixels, int width, int height)
{
	Pixels** image_res = init(width, height);
	int max_r, max_g, max_b, sum_r, sum_g, sum_b;
	Pixels p;
	int N = tabPixels.size();
	for (size_t x = 0; x < height; x++)
	{
		for (size_t y = 0; y < width; y++)
		{
			sum_r = 0;
			sum_g = 0;
			sum_b = 0;

			for (Pixels** pixels : tabPixels) 
			{
				p = pixels[x][y];

				sum_r += p.red;
				sum_g += p.green;
				sum_b += p.blue;
			}


			max_r = int(sum_r / N);
			max_g = int(sum_g / N);
			max_b = int(sum_b / N);

			image_res[x][y] = { max_r, max_g, max_b, 255 };
		}
	}
	return image_res;
}

Pixels** Fading_front(std::list<Pixels**> tabPixels, Pixels** Mediane,int width, int height)
{
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tempPix;
	tempPix.push_back(tabPixels.front());
	tabPixels.pop_front();
	int nbtabPixels = tabPixels.size();
	for (size_t i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.front());
		tabPixels.pop_front();
		res_moy = MoyenneNimages(tempPix, width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.back(), width, height);
	res_moy = AppliquerMasque(res_moy, tabPixels.back(), masque, width, height);

	return res_moy;
}

Pixels** Fading_back(std::list<Pixels**> tabPixels, Pixels** Mediane, int width, int height)
{
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tempPix;
	tempPix.push_back(tabPixels.back());
	tabPixels.pop_back();
	int nbtabPixels = tabPixels.size();
	for (size_t i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.back());
		tabPixels.pop_back();
		res_moy = MoyenneNimages(tempPix, width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.front(), width, height);
	res_moy = AppliquerMasque(res_moy, tabPixels.front(), masque, width, height);

	return res_moy;
}

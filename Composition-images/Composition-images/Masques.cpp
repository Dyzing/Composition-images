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
			tab[i][j] = { 0,0,0 };

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


Pixels** MultiMasque(Pixels** Mediane, Image* tabImage, int nb, Pixels** fond, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = fond;
	Pixels** pixels;
	for (int x = 0; x < nb; ++x) {
		pixels = tabImage[x].getTabPixels();
		masque = CreationMasque(Mediane, pixels, width, height, x);
		resFinal = AppliquerMasque(resFinal, pixels, masque, width, height);
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

			image_res[x][y] = { max_r, max_g, max_b};
		}
	}
	return image_res;
}

Pixels** Fading_front(Image* tabImage, Pixels** Mediane, int nb, int width, int height)
{
	
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tabPixels;
	std::list<Pixels**> tempPix;
	for (int x = 0; x < nb; ++x) {
		tabPixels.push_back(tabImage[x].getTabPixels());
	}
	tempPix.push_back(tabPixels.front());
	tabPixels.pop_front();
	int nbtabPixels = tabPixels.size();
	for (int i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.front());
		tabPixels.pop_front();
		res_moy = MoyenneNimages(tempPix, width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.back(), width, height, 101);
	res_moy = AppliquerMasque(res_moy, tabPixels.back(), masque, width, height);

	return res_moy;
}

Pixels** Fading_back(Image* tabImage, Pixels** Mediane, int nb, int width, int height)
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
		res_moy = MoyenneNimages(tempPix, width, height);
		tempPix.clear();
		tempPix.push_back(res_moy);
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.front(), width, height, 102);
	res_moy = AppliquerMasque(res_moy, tabPixels.front(), masque, width, height);

	return res_moy;
}

/*
int cc_size(Pixels** im, int width, int height, int x, int y)
{
	std::tuple<int, int> start = { x, y };
	std::stack< std::tuple<int, int> > deque;
	int compteur = 0;

	if (!(im[x][y].red <= 25 && im[x][y].green <= 25 && im[x][y].blue <= 25)) //a changer si on veut faire en fontion de la tolérance
	{
		compteur = 1;
		deque.push(start);
		im[x][y] = { 0, 0, 0, 255 };
		while (deque.size() > 0)
		{
			x = std::get<0>(deque.top());
			y = std::get<1>(deque.top());
			deque.pop();

			for (int i = x - 1; i < x + 2; i++)
			{
				if (i >= 0 && i < height)
				{
					for (int j = y - 1; j < y + 2; j++)
					{
						if (j >= 0 && j < width)
						{
							if (i != x || j != y)
							{
								if (!(im[i][j].red <= 25 && im[i][j].green <= 25 && im[i][j].blue <= 25))
								{
									deque.push({ i, j });
									im[i][j] = { 0, 0, 0, 255 };
									++compteur;
								}
							}
						}
					}
				}
			}
		}
	}
	return compteur;
}

Pixels** cc_size_tab(Pixels** im, int width, int height, int x, int y)
{
	std::tuple<int, int> start = { x, y };
	std::stack< std::tuple<int, int> > deque;
	Image* tmp = new Image(width, height);
	Pixels** tab = tmp->getTabPixels();
	int compteur = 0;

	if (!(im[x][y].red <= 25 && im[x][y].green <= 25 && im[x][y].blue <= 25)) //a changer si on veut faire en fontion de la tol�rance
	{
		compteur = 1;
		deque.push(start);
		im[x][y] = {0, 0, 0, 255};
		while (deque.size() > 0)
		{
			x = std::get<0>(deque.top());
			y = std::get<1>(deque.top());
			deque.pop();

			for (int i = x-1; i < x+2; i++)
			{
				if (i >= 0 && i < height)
				{
					for (int j = y-1; j < y+2; j++)
					{
						if (j >= 0 && j < width)
						{
							if (i != x || j != y)
							{
								if (!(im[i][j].red <= 25 && im[i][j].green <= 25 && im[i][j].blue <= 25))
								{
									deque.push({ i, j });
									tab[i][j] = im[i][j];
									im[i][j] = { 0, 0, 0, 255 };
									++compteur;
								}
							}
						}
					}
				}
			}
		}
	}
	return tab;
}

Pixels** remove_cc(Pixels** im, int width, int height, int x, int y)
{
	std::tuple<int, int> start = { x, y };
	std::stack< std::tuple<int, int> > deque;




Pixels** filter_cc(Pixels** im, int width, int height, int minSize)
{
	Image* cop = new Image(width, height);
	cop->setTabPixels(im);
	Pixels** pix2 = cop->getTabPixels();
	int nb_cc_size;

	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			nb_cc_size = cc_size(pix2, width, height, x, y);
			if (0 < nb_cc_size && nb_cc_size <= minSize)
			{
				remove_cc(im, width, height, x, y);
			}
		}
	}
	return im;
}

Pixels** biggest_cc(Pixels** im, int width, int height)
{
	Image* cop = new Image(width, height);
	cop->setTabPixels(im);
	Pixels** pix2 = cop->getTabPixels();
	int biggersize = 0;
	int currentsize;
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			currentsize = cc_size(pix2, width, height, x, y);
			if (currentsize > biggersize)
			{
				biggersize = currentsize;
			}
		}
	}
	return filter_cc(im, width, height, biggersize);
}
*/

void InitTabBool(bool* tab, int const& nb)
{
	for (int i = 0; i < nb; i++)
	{
		tab[i] = false;
	}
}

void InittabNbOver(int* tabNbOver, int const& nb)
{
	for (int i = 0; i < nb; i++)
	{
		tabNbOver[i] = 0;
	}
}


Pixels** overlap(Pixels** Mediane, Image* tabImage, int nb, Pixels**fond, int width, int height, int overmax)
{
	Pixels** tabpixelMasques;
	Pixels** tabpixelCourant;
	bool * tabMasqueValide = new bool[nb];
	int * tabNbOver = new int[nb];
	bool* tabDejaVu = new bool[nb];
	InitTabBool(tabMasqueValide, nb);
	InittabNbOver(tabNbOver, nb);
	InitTabBool(tabDejaVu, nb);
	int indexImageValide = 0;

	Image* ImagesValides = new Image[nb]; //remplit la liste d'image
	std::vector<Pixels**> imageMasquee;

	Pixels** masque;

	for (int i = 0; i < nb; i++)
	{
		InitTabBool(tabDejaVu, nb);
		tabMasqueValide[i] = true;
		masque = CreationMasque(Mediane, tabImage[i].getTabPixels(), width, height, i);
		imageMasquee.push_back(masque);
		tabpixelCourant = imageMasquee[i];
		int x = 0;
		while (x < height && tabMasqueValide[i])
		{
			int y = 0;
			while (y < width && tabMasqueValide[i])
			{
				if (tabpixelCourant[x][y] > 15)
				{
					for (int j = 0; j < i; j++)
					{
						if (tabMasqueValide[j])
						{
							tabpixelMasques = imageMasquee[j];
							if (tabDejaVu[j] == false)
							{
								if (tabpixelMasques[x][y] > 15)
								{
									//intersection
									if (tabNbOver[j] + 1 <= overmax)
									{
										++tabNbOver[j];
										++tabNbOver[i];
										tabDejaVu[j] = true;
									}
									else
									{
										tabNbOver[i] = 0;
										tabMasqueValide[i] = false;
									}
								}
							}
						}
					}
					if (tabDejaVu[i] == false)
					{
						++tabNbOver;
						tabDejaVu[i] = true;
					}
				}
				++y;
			}
			++x;
		}
		if (tabMasqueValide[i])
		{
			ImagesValides[indexImageValide] = tabImage[i];
			indexImageValide += 1;
		}
	}

	Pixels** resultat = MultiMasque(Mediane, ImagesValides, indexImageValide, fond, width, height);

	return resultat;
}

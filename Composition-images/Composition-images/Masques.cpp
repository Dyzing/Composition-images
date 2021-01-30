#include "Masques.hpp"
#include <vector>
#include <numeric>
#include <stack>
#include <tuple>

Pixels** CreationMasque(Pixels** Fond, Pixels** Image, int width, int height) {
	Image = FlouGaussien(Image, width, height);
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

Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height) {

	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!(Masque[i][j] <= 12 )) {
				tabFinal[i][j] = ImgBase[i][j];
			}
			else {
				tabFinal[i][j] = Fond[i][j];
			}
			
		}
	}
	return tabFinal;
}


Pixels** MultiMasque(Pixels** Mediane, std::list<Pixels**> tabPixels, Pixels** fond, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = fond;
	corona::Image* img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	for (Pixels** tab : tabPixels) {
		masque = CreationMasque(Mediane, tab, width, height);
		resFinal = AppliquerMasque(resFinal, tab, masque, width, height);
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

			for (size_t i = x - 1; i < x + 2; i++)
			{
				if (i >= 0 && i < height)
				{
					for (size_t j = y - 1; j < y + 2; j++)
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
	Pixels** tab = init(width, height);
	int compteur = 0;

	if (!(im[x][y].red <= 25 && im[x][y].green <= 25 && im[x][y].blue <= 25)) //a changer si on veut faire en fontion de la tolérance
	{
		compteur = 1;
		deque.push(start);
		im[x][y] = {0, 0, 0, 255};
		while (deque.size() > 0)
		{
			x = std::get<0>(deque.top());
			y = std::get<1>(deque.top());
			deque.pop();

			for (size_t i = x-1; i < x+2; i++)
			{
				if (i >= 0 && i < height)
				{
					for (size_t j = y-1; j < y+2; j++)
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

	if (!(im[x][y].red <= 25 && im[x][y].green <= 25 && im[x][y].blue <= 25)) //a changer si on veut faire en fontion de la tolérance
	{
		deque.push(start);
		im[x][y] = { 0, 0, 0, 255 };
		while (deque.size() > 0)
		{
			x = std::get<0>(deque.top());
			y = std::get<1>(deque.top());
			deque.pop();

			for (size_t i = x - 1; i < x + 2; i++)
			{
				if (i >= 0 && i < height)
				{
					for (size_t j = y - 1; j < y + 2; j++)
					{
						if (j >= 0 && j < width)
						{
							if (i != x || j != y)
							{
								if (!(im[i][j].red <= 25 && im[i][j].green <= 25 && im[i][j].blue <= 25))
								{
									deque.push({ i, j });
									im[i][j] = { 0, 0, 0, 255 };
								}
							}
						}
					}
				}
			}
		}
	}
	return im;
}

Pixels** filter_cc(Pixels** im, int width, int height, int minSize)
{
	corona::Image* cop = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(im, cop);
	Pixels** pix2 = ImageToPixels(cop);
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
	corona::Image* cop = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(im, cop);
	Pixels** pix2 = ImageToPixels(cop);
	int biggersize = 0;
	int currentsize;
	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y <  width; y++)
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


//connexe a effacer
//corona::Image* connexe_jpeg = corona::OpenImage("../Photos/test.jpg", corona::PF_R8G8B8A8);
//corona::Image* connexe_jpeg_clone = corona::CloneImage(connexe_jpeg, corona::PF_R8G8B8A8);
//Pixels** tab_connexe_jpeg = ImageToPixels(connexe_jpeg); //pour le cc_size_tab
//Pixels** tab_connexe_jpeg_clone = ImageToPixels(connexe_jpeg_clone); //pour le cc_size


//int cc_size_number = cc_size(tab_connexe_jpeg_clone, connexe_jpeg_clone->getWidth(), connexe_jpeg_clone->getHeight(), 400, 100);
//std::cout << "---------------------" << std::endl << "nb connexe : " << cc_size_number << std::endl << "---------------------" << std::endl;

/*Pixels** cc_size_tab_image = cc_size_tab(tab_connexe_jpeg, connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), 400, 100);
corona::Image* connexe_jpg_new = corona::CreateImage(connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), corona::PF_R8G8B8A8);
TabToPixels(cc_size_tab_image, connexe_jpg_new);
corona::SaveImage("../Photos/test_connexe.jpg", corona::FileFormat::FF_PNG, connexe_jpg_new);

std::cout << "---------------------" << std::endl << "test_connexe termine" << std::endl << "---------------------" << std::endl;*/

//Pixels** cc_filter_image = filter_cc(tab_connexe_jpeg, connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), 20000);
//corona::Image* cc_filter_jpg = corona::CreateImage(connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), corona::PF_R8G8B8A8);
//TabToPixels(cc_filter_image, cc_filter_jpg);
//corona::SaveImage("../Photos/test_filter.jpg", corona::FileFormat::FF_PNG, cc_filter_jpg);

//std::cout << "---------------------" << std::endl << "test_filter termine" << std::endl << "---------------------" << std::endl;

//Pixels** cc_biggest_image = biggest_cc(tab_connexe_jpeg, connexe_jpeg->getWidth(), connexe_jpeg->getHeight());
//corona::Image* cc_biggest_jpg = corona::CreateImage(connexe_jpeg->getWidth(), connexe_jpeg->getHeight(), corona::PF_R8G8B8A8);
//TabToPixels(cc_biggest_image, cc_biggest_jpg);
//corona::SaveImage("../Photos/test_biggest.jpg", corona::FileFormat::FF_PNG, cc_biggest_jpg);

//std::cout << "---------------------" << std::endl << "test_biggest termine" << std::endl << "---------------------" << std::endl;


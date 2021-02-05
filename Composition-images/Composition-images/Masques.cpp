#include "Masques.hpp"
#include<string>
#include <tuple>
#include<vector>
#include <numeric>
#include <stack>
#include <tuple>

Pixels** CreationMasque(Pixels** Fond, Pixels** Img, int width, int height, int n) {
	Img = FlouGaussien(Img, width, height); // Flou gaussien appliqué pour correspondre à la médiane
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tabFinal[i][j].red = std::abs(Fond[i][j].red - Img[i][j].red); //je fais la différence absolue de chaque composantes des pixels afin d'avoir une image différence entre l'image de fond et l'image avec un seul personnage pour récuperer le masque. 
			tabFinal[i][j].green = std::abs(Fond[i][j].green - Img[i][j].green);
			tabFinal[i][j].blue = std::abs(Fond[i][j].blue - Img[i][j].blue);
		}
	}
	tabFinal = plusGrandConnexe(tabFinal, height, width); //je fais le plusgrandConnexe pour enlever le plus de bruit possible et de garder au maximuim seulement le masque
	return tabFinal;
}


int tailleConnexe(Pixels** tab, Pixels** copyTab, int height, int width, int x, int y) {

	std::tuple<int, int> t;
	std::vector<std::tuple<int, int>> v;
	v.push_back(std::make_tuple(x, y));
	int count = 0;
	int i, j;
	Pixels p;
	while (!v.empty()) {//tant qu'il y a des point dans notre pile
		t = v.back();
		v.pop_back();
		i = std::get<0>(t);
		j = std::get<1>(t);
		p = tab[i][j];
		if (!(p <= tolerance)) { //si notre pixel n'est pas noir(plus ou moins la tolerance)
			count += 1;
			tab[i][j] = { 0,0,0 };//on met notre pixel en noir pour eviter d'y retourner

			copyTab[i][j] = p;//on copie le pixel
			for (int n = -1; n < 2; n++) {
				if (!(i + n > height - 1 or i + n < 0)) {
					for (int k = -1; k < 2; k++) {
						if (!(j + k > width - 1 or j + k < 0)) {
							v.push_back(std::make_tuple(i + n, j + k));//on empile les quatre coordoné autour de notre pixel (si elles sont dans l'image)
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
			//pour chaque pixels de l'image
			p = tab[i][j];
			if (!(p <= tolerance)) {//si notre pixel n'est pas noir(plus ou moins la tolerance)
				for (int k = 0; k < height; k++) {
					for (int n = 0; n < width; n++) {
						connex[k][n] = { 0, 0, 0};
					}
				}
				taille = tailleConnexe(tab, connex, height, width, i, j);//on recupere la composante et sa taille 
				if (taille > max) {//si elle est plus grande que la plus grande
					max = taille;
					for (int k = 0; k < height; k++)
					{
						for (int n = 0; n < width; n++) {
							connexMax[k][n] = connex[k][n];// on save la composante
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
	//La moyenne n image fonctionne ainsi : je récupère les valeurs de chaque pixel dans chaque image à chaque points donnés et j'en fait la moyenne dans l'image résultat.
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
				//je fais la somme de chaque composantes des pixels
				sum_r += p.red;
				sum_g += p.green;
				sum_b += p.blue;
			}

			//je fais la moyenne de chaque composantes des pixels
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
	//Le principe des Fading est que je fais une moyenne des 2 premières images que je stocke dans un autre image temporaire.
	//Puis je fais une autre moyenne avec la troisième et l'image temporaire. Je fais ce traitement ainsi de suite afin d'avoir des images de plus en plus moyennées.
	//Ainsi, je me retrouve avec des images de plus en plus transparentes. On a choisi de mettre la première ou la dernière en opaque histoire d'avoir un rendu plus logique et agréable.
	//Dans le cas du Fading_front, je récupère les fronts de chaque liste.
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tabPixels;
	std::list<Pixels**> tempPix; //tempPix équivaut à la liste des images à moyenner que je renitialise à chaque tour et que je re push_back le résultat de la moyenne calculée.
	for (int x = 0; x < nb; ++x) {
		tabPixels.push_back(tabImage[x].getTabPixels());
	}
	tempPix.push_back(tabPixels.front()); //j'ajoute l'image d'après dans le temPix afin de pouvoir calculer la moyenne avec l'image déjà présente.
	tabPixels.pop_front();
	int nbtabPixels = tabPixels.size();
	for (int i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.front());
		tabPixels.pop_front();
		res_moy = MoyenneNimages(tempPix, width, height); //fait la moyenne des deux éléments dans le tempPix
		tempPix.clear();
		tempPix.push_back(res_moy); //je met le résultat de la moyenne dans le temPix pour ainsi calculer la moyenne de l'image d'après avec la moyenne calculée.
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.back(), width, height, 101);
	res_moy = AppliquerMasque(res_moy, tabPixels.back(), masque, width, height);

	return res_moy;
}

Pixels** Fading_back(Image* tabImage, Pixels** Mediane, int nb, int width, int height)
{
	//Le principe des Fading est que je fais une moyenne des 2 premières images que je stocke dans un autre image temporaire.
	//Puis je fais une autre moyenne avec la troisième et l'image temporaire. Je fais ce traitement ainsi de suite afin d'avoir des images de plus en plus moyennées.
	//Ainsi, je me retrouve avec des images de plus en plus transparentes. On a choisi de mettre la première ou la dernière en opaque histoire d'avoir un rendu plus logique et agréable.
	//Dans le cas du Fading_front, je récupère les backs de chaque liste.
	Pixels** res_moy = init(width, height);
	std::list<Pixels**> tabPixels;
	std::list<Pixels**> tempPix; //tempPix équivaut à la liste des images à moyenner que je renitialise à chaque tour et que je re push_back le résultat de la moyenne calculée.
	for (int x = 0; x < nb; ++x) {
		tabPixels.push_back(tabImage[x].getTabPixels());
	}
	tempPix.push_back(tabPixels.back());
	tabPixels.pop_back();
	int nbtabPixels = tabPixels.size();
	for (int i = 0; i < nbtabPixels - 1; i++)
	{
		tempPix.push_back(tabPixels.back()); //j'ajoute l'image d'après dans le temPix afin de pouvoir calculer la moyenne avec l'image déjà présente.
		tabPixels.pop_back();
		res_moy = MoyenneNimages(tempPix, width, height); //fait la moyenne des deux éléments dans le tempPix
		tempPix.clear();
		tempPix.push_back(res_moy); //je met le résultat de la moyenne dans le temPix pour ainsi calculer la moyenne de l'image d'après avec la moyenne calculée.
	}

	Pixels** masque = CreationMasque(Mediane, tabPixels.front(), width, height, 102);
	res_moy = AppliquerMasque(res_moy, tabPixels.front(), masque, width, height);

	return res_moy;
}


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
	int indexImageValide = 0;

	Image* ImagesValides = new Image[nb]; //tableau des images valides selon le critère
	std::vector<Pixels**> imageMasquee; //vecteur permettant de stocker les masques à utiliser

	Pixels** masque;

	for (int i = 0; i < nb; i++)
	{
		InitTabBool(tabDejaVu, nb);
		tabMasqueValide[i] = true;
		masque = CreationMasque(Mediane, tabImage[i].getTabPixels(), width, height, i); //création des masques au fur et à mesure de l'algorithme
		imageMasquee.push_back(masque);
		tabpixelCourant = imageMasquee[i];
		int x = 0;
		while (x < height && tabMasqueValide[i])
		{
			int y = 0;
			while (y < width && tabMasqueValide[i])
			{
				if (tabpixelCourant[x][y] > 15) //si le pixel du masque courant fait partie de la silhouette du personnage
				{
					for (int j = 0; j < i; j++)
					{
						if (tabMasqueValide[j])
						{
							tabpixelMasques = imageMasquee[j];
							if (tabDejaVu[j] == false) //si j'ai déjà traité une intersection avec le j-ième masque
							{
								if (tabpixelMasques[x][y] > 15) //si il y a une intersection avec le masque courant et le j-ième masque
								{
									if (tabNbOver[j] + 1 <= overmax) //si cette intersection ne dépasse pas le nombre de superposition autorisé par overmax
									{
										++tabNbOver[j];
										++tabNbOver[i];
										tabDejaVu[j] = true;
									}
									else //sinon je ne valide pas ce masque et ne sera plus utilisé pour la suite du traitement de l'algorithme
									{
										tabNbOver[i] = 0;
										tabMasqueValide[i] = false;
									}
								}
							}
						}
					}
					if (tabDejaVu[i] == false) //si j'ai déjà trouvé au moins 1 pixel dans le masque courant, ainsi j'augmente sa superposition de 1 (superposition minimale logique)
					{
						++tabNbOver[i];
						tabDejaVu[i] = true;
					}
				}
				++y;
			}
			++x;
		}
		if (tabMasqueValide[i]) //si mon masque à la fin de son traitement est valide, je rentre son image associée dans le tableau des images valide
		{
			ImagesValides[indexImageValide] = tabImage[i];
			indexImageValide += 1;
		}
	}

	Pixels** resultat = MultiMasque(Mediane, ImagesValides, indexImageValide, fond, width, height); //j'applique les masques et les images sur l'image de fond selon le tableau des images valide

	return resultat;
}

struct minmax {
	int ymin;
	int ymax;
};
Pixels** distance(Pixels** Mediane, Image* tabImage, int nb, Pixels** fond, int width, int height, int distMin) {

	Pixels** resultat;
	Pixels** masque;
	std::vector<minmax> minmaxdesMasques;
	int xmin = height, ymin = width, xmax = 0, ymax = 0;
	bool* tabMasqueValide = new bool[nb];
	InitTabBool(tabMasqueValide, nb);
	tabMasqueValide[0] = true;
	int indexImageValide = 0;
	masque = CreationMasque(Mediane, tabImage[0].getTabPixels(), width, height, 0);
	for (int x = 0; x < height; ++x) {
		for (int y = 0; y < width; ++y) {
			
			if (masque[x][y] > 5) {
				if (x < xmin)
					xmin = x;
				if (x > xmax)
					xmax = x;
				if (y < ymin)
					ymin = y;
				if (y > ymax)
					ymax = y;

			}
		}
	} // Le premier masque est forcément valide car il n'a pas de sujets a tester pour la distance
	++indexImageValide;
	minmaxdesMasques.push_back({ymin,ymax });
	for (int i = 1; i < nb; ++i) {
		masque = CreationMasque(Mediane, tabImage[i].getTabPixels(), width, height, i);
		xmax, ymax = 0;
		xmin = height;
		ymin = width;
		for (int x = 0; x < height; ++x) {
			for (int y = 0; y < width; ++y) {
				if (masque[x][y] > 13) {
					if (y < ymin)
						ymin = y;
					if (y > ymax)
						ymax = y;

				}
			}
		}
		minmaxdesMasques.push_back({ymin,ymax }); // On récupère les minimums et maximums de chaque sujets
		bool toAdd = true;
		for (int ind = 0; ind < nb; ++ind) {
			if (i != ind) {
				if (tabMasqueValide[ind] != false) {
					if ((std::abs(minmaxdesMasques[i].ymax - minmaxdesMasques[ind].ymin) < distMin || std::abs(minmaxdesMasques[ind].ymax - minmaxdesMasques[i].ymin) < distMin)) { // Si la distance entre les sujets est inférieur a la distance minimale, il sera retirer
						toAdd = false;
						break;
					}
				}
			}
			
		}
		if (toAdd) {
			tabMasqueValide[i] = true;
			++indexImageValide;
		}
	}

	
	Image* ImagesValides = new Image[indexImageValide];
	int cpt = 0;
	for (int i = 0; i < nb; ++i) {
		if (tabMasqueValide[i] == true) {
			ImagesValides[cpt] = tabImage[i];  // On ajoute les images qui sont valides par la distance
			++cpt;
		}
			
	}

	resultat = MultiMasque(Mediane, ImagesValides, indexImageValide, fond, width, height); // Application des masques valides sur l'image


	return resultat;


}
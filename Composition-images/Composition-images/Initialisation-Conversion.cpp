#include "Initialisation-Conversion.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <list>
Pixels * *init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [hauteur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = &tab2[i * largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 255,255,255,255 };
		}
	}
	return tab;
}

corona::Image** initImage(std::list<std::string> arguments) {
	corona::Image* img;
	int n = arguments.size();
	corona::Image** tabImage = new corona::Image * [n];
	for (int i = 0; i < n; ++i) {
		img = corona::OpenImage(arguments.front().c_str(), corona::PF_R8G8B8A8);
		arguments.pop_front();
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



bool validPATH(std::string PATH)               
{
	const char* st = PATH.c_str();
	if (PATH.length() == 2)
	{
		if (st[1] == ':')
			return true;
	}
	else
	{
		if (st[1] == ':' && st[2] == '\\' && st[PATH.length()] != '\\')// Le chemin doit commencer avec 1 lettre uniquement suivi de :\\ et la fin ne doit pas finir par '\\'
			return true;
		if (st[0] == '.' && st[2] == '\\' && st[PATH.length()] != '\\')
			return true;
		if (st[0] == '.' && st[1] == '.' && st[2] == '\\' && st[PATH.length()] != '\\')
			return true;
		return false;
	}
}

std::list<std::string> FindFilesInDirectory(std::string dir) {
	std::string sT,sP;
	if (validPATH(dir))
	{
		sT = dir;
		sT.append("\\");
	}
	else
		std::cout << "INVALID PATH! ABORTING...", exit(EXIT_SUCCESS);
	WIN32_FIND_DATAA FindFileData;
	HANDLE hF;
	sP = sT;
	sP.append("\\");
	sT.append("*");
	std::list<std::string> lst;
	hF = FindFirstFileA(sT.data(), &FindFileData);
	do {
		lst.push_back(sP + FindFileData.cFileName);
	} while (FindNextFileA(hF, &FindFileData));
	FindClose(hF);
	lst.pop_front();
	lst.pop_front();
	return lst;
}

void getParams(int argc, char* argv[], std::list<std::string>& files, std::string& fading) {

	//Recuperation de fichiers à partir d'un dossier
	if (cmdOptionExists(argv, argv + argc, "-dir"))
	{
		char* filename = getCmdOption(argv, argv + argc, "-dir");
		files = FindFilesInDirectory(filename);

	}

	//Recuperation de la liste des fichiers 
	else if (cmdOptionExists(argv, argv + argc, "-file")) {
		char** end = argv + argc;
		char** debut = argv;
		const std::string option = "-file";
		char** itr = std::find(debut, end, option);
		if (itr != argv + argc && itr + 1 != argv + argc) {
			++itr;
			while (itr != argv + argc) {

				files.push_back(*itr);
				++itr;
			}
		}
	}
	//Recuperation de la video
	if (cmdOptionExists(argv, argv + argc, "-video")) {
		std::cout << "NOT IMPLEMENTED! ABORTING...", exit(EXIT_SUCCESS);
	}
	//Recuperation de l'option fading
	if (cmdOptionExists(argv, argv + argc, "-fading")) {
		char* tmp = getCmdOption(argv, argv + argc, "-fading");
		std::string fadingOpt = tmp;
		if ((fadingOpt == "opaque") or (fadingOpt == "plus") or (fadingOpt == "moins")) {
			fading = fadingOpt;
		}
	}

	
}
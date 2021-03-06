﻿#include "Initialisation-Conversion.hpp"


Pixels** init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [hauteur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = new Pixels[largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0};
		}
	}
	return tab;
}
Image* initImage(std::list<std::string> arguments) {
	Image* img;
	int n = arguments.size();
	Image* tabImage = new Image[n];
	for (int i = 0; i < n; ++i) {
		img = new Image(arguments.front().c_str());
		arguments.pop_front();
		tabImage[i] = *img;
	}

	return tabImage;
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
	return false;
}
std::list<std::string> FindFilesInDirectory(std::string dir) {
	std::string sT,sP;
	if (validPATH(dir))
	{
		sT = dir;
		sT.append("\\");
	}
	else
		std::cout << "INVALID PATH! ABORTING...", exit(EXIT_SUCCESS); // Chemin de Dossier invalide
	WIN32_FIND_DATAA FindFileData; // Pointeur pour les fichiers
	HANDLE hF;
	sP = sT;
	sP.append("\\");
	sT.append("*"); // complete le chemin pour tout recuperer
	std::list<std::string> lst;
	hF = FindFirstFileA(sT.data(), &FindFileData); // Trouve le premier fichier du repertoire
	do {
		lst.push_back(sP + FindFileData.cFileName);
	} while (FindNextFileA(hF, &FindFileData)); // Ajoute le nom des fichiers à la liste des fichiers
	FindClose(hF);
	lst.pop_front();
	lst.pop_front(); // Retire le dossier courant ainsi que le dossier parent (. et ..)
	return lst; // Retourne la liste des fichiers
}
char* getCmdOption(char** begin, char** end, const std::string& option)
{
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{	
	return std::find(begin, end, option) != end;
}

void getParams(int argc, char* argv[], std::list<std::string>& files, std::string& fading, int& overlap, int& distance, bool& parDefaut) {

	//Recuperation de fichiers a partir d'un dossier
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
			while (itr != argv + argc -1) {
				files.push_back(*itr);
				++itr;
			}
		}
	}
	//Recuperation de la video
	if (cmdOptionExists(argv, argv + argc, "-video")) {
		std::cout << "NOT IMPLEMENTED! ABORTING THE MISSION", exit(EXIT_SUCCESS);
	}
	//Recuperation de l'option fading
	if (cmdOptionExists(argv, argv + argc, "-fading")) {
		char* tmp = getCmdOption(argv, argv + argc, "-fading");
		std::string fadingOpt = tmp;
		if ((fadingOpt == "opaque") or (fadingOpt == "plus") or (fadingOpt == "moins")) {
			fading = fadingOpt;
		}
		parDefaut = false;
	}
	//Recuperation de l'option overlap
	if (cmdOptionExists(argv, argv + argc, "-overlap")) {
		std::string overlapOpt = getCmdOption(argv, argv + argc, "-overlap");
		overlap = std::stoi(overlapOpt);
		parDefaut = false;
	}

	//Recuperation de l'option overlap
	if (cmdOptionExists(argv, argv + argc, "-distance")) {
		std::string distanceOpt = getCmdOption(argv, argv + argc, "-distance");
		distance = std::stoi(distanceOpt);
		parDefaut = false;
	}
	//Recuperation de l'option overlap
	if (cmdOptionExists(argv, argv + argc, "-tolerance")) {
		std::string toleranceOpt = getCmdOption(argv, argv + argc, "-tolerance");
		tolerance = std::stoi(toleranceOpt);
	}

	
}
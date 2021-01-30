#pragma once

#include "corona.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <list>

/// <summary>
/// Recuperation de la liste d'image passe en parametres
/// </summary>
/// <param name="n">Nombre d'image</param>
/// <param name="arguments">chemin des images</param>
/// <returns>Tableau comportant toutes les images</returns>
Image* initImage(std::list<std::string> arguments);




/// <summary>
/// Valide ou non le chemin passé en parametre
/// </summary>
/// <param name="PATH"></param>
/// <returns>True : valide / false : non valide</returns>
bool validPATH(std::string PATH);
/// <summary>
/// Trouve tout les fichiers correspondant au repertoire
/// </summary>
/// <param name="dir">Chemin du repertoire</param>
/// <returns>Liste des fichiers du repertoire</returns>
std::list<std::string> FindFilesInDirectory(std::string dir);


/// <summary>
/// Recupère les paramètres de l'application
/// </summary>
/// <param name="argc">Nombre d'arguments</param>
/// <param name="argv">Liste d'arguments</param>
/// <param name="files">Liste des fichiers qui sera mis a jour</param>
/// <param name="fading">Option fading</param>
void getParams(int argc, char* argv[], std::list<std::string>& files, std::string& fading);
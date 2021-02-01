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
/// Initialise un tableau de pixels
/// </summary>
/// <param name="largeur">Largeur</param>
/// <param name="hauteur">Hauteur</param>
/// <returns>Retourne un tableau initialise</returns>
Pixels** init(int largeur, int hauteur);

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
/// Test si l'option existe
/// </summary>
/// <param name="begin">Debut Arguments</param>
/// <param name="end">Fin Arguments</param>
/// <param name="option">Option a chercher</param>
/// <returns>Trouver ou non</returns>
bool cmdOptionExists(char** begin, char** end, const std::string& option);
/// <summary>
/// Recupere la valeur de l'option
/// </summary>
/// <param name="begin">Debut Arguments</param>
/// <param name="end">Fin Arguments</param>
/// <param name="option">Option a chercher</param>
/// <returns>Valeur de l'option</returns>
char* getCmdOption(char** begin, char** end, const std::string& option);

/// <summary>
/// Recupère les paramètres de l'application
/// </summary>
/// <param name="argc">Nombre d'arguments</param>
/// <param name="argv">Liste d'arguments</param>
/// <param name="files">Liste des fichiers qui sera mis a jour</param>
/// <param name="fading">Option fading</param>
/// <param name="overlap">Option overlap(Max de superpositions)</param>
/// <param name="fading">Option distance(Distance entre 2 sujets)</param>
void getParams(int argc, char* argv[], std::list<std::string>& files, std::string& fading, int& overlap, int& distance, bool& parDefaut);
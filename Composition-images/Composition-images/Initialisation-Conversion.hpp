#pragma once

#include "corona.hpp"
/// <summary>
/// Initialisation d'un tableau
/// </summary>
/// <param name="hauteur"></param>
/// <param name="largeur"></param>
/// <returns></returns>
Pixels * *init(int largeur, int hauteur);

/// <summary>
/// Recuperation de la liste d'image passe en parametres
/// </summary>
/// <param name="n">Nombre d'image</param>
/// <param name="arguments">chemin des images</param>
/// <returns>Tableau comportant toutes les images</returns>
corona::Image** initImage(std::list<std::string> arguments);

/// <summary>
/// Initialisation des tableaux correspondant a la liste d'images
/// </summary>
/// <param name="taille">Nombre d'images</param>
/// <param name="tabImg">Liste d'images</param>
/// <returns>Liste des tableaux de pixels</returns>
std::list<Pixels**> initTabPixels(int taille, corona::Image** tabImg);

/// <summary>
/// Convertit une image en tableau de pixels
/// </summary>
/// <param name="img">Image a convertir</param>
/// <returns>Tableau de pixels</returns>
Pixels** ImageToPixels(corona::Image* img);

/// <summary>
/// Transfert le tableau de pixels dans l'image
/// </summary>
/// <param name="pix">Tableau de pixels</param>
/// <param name="img">Image qui recevra le tableau de pixels</param>
void TabToPixels(Pixels** pix, corona::Image* img);


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
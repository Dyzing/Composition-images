#pragma once
#include "Initialisation-Conversion.hpp"

/// <summary>
/// Application du Flou Gaussien
/// </summary>
/// <param name="img">Image sur laquelle on souhaite utiliser le flou</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Image floutee par le Flou Gaussien</returns>
Pixels * *FlouGaussien(Pixels * *img, int width, int height);

/// <summary>
/// Creation de la mediane / extraction du sujet des images
/// </summary>
/// <param name="images">Liste d'image</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns></returns>
Pixels** median_images(std::list<Pixels**> images, int width, int height);
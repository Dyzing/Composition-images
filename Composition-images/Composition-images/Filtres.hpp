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
/// Application du Filtre Median
/// </summary>
/// <param name="image">Image sur laquelle on souhaite utiliser le flou</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <param name="radius">Radius</param>
/// <returns>Image debruite avec le filtre </returns>
Pixels** filtre_median(Pixels** image, int width, int height, int radius);

/// <summary>
/// Creation de la mediane / extraction du sujet des images
/// </summary>
/// <param name="images">Liste d'image</param>
/// <param name="nb">Nombre d'images</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Mediane des images</returns>
Pixels** median_images(Image* images,int nb, int width, int height);
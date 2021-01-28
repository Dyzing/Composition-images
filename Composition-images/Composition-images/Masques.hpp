#pragma once

#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"


/// <summary>
/// Creation du masque correspondant au sujet de l'image
/// </summary>
/// <param name="Fond">Fond de l'image sans le sujet</param>
/// <param name="Image">Image ou seras extrait le sujet</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Retourne le masque</returns>
Pixels** CreationMasque(Pixels * *Fond, Pixels * *Image, int width, int height);
/// <summary>
/// Permet l'application du masque a partir de l'image de base sur le Fond
/// </summary>
/// <param name="Fond">Fond de l'image sans le sujet</param>
/// <param name="ImgBase">Image de base comportant le fond et le sujet</param>
/// <param name="Masque">Masque correspondant au sujet sur l'image de base</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height);
/// <summary>
/// Fonction appliquant tout les masques correspondant aux images passes en parametre
/// </summary>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="tabPixels">Liste des images sous formes de tableau de pixels</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Resultat final comportant le fond + les sujets</returns>
Pixels** MultiMasque(Pixels** Mediane, std::list<Pixels**> tabPixels, int width, int height);
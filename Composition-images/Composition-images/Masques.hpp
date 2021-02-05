#pragma once
#include "Initialisation-Conversion.hpp"
#include "Filtres.hpp"
#include <algorithm>


/// <summary>
/// Creation du masque correspondant au sujet de l'image
/// </summary>
/// <param name="Fond">Fond de l'image sans le sujet</param>
/// <param name="Image">Image ou seras extrait le sujet</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Retourne le masque</returns>
Pixels** CreationMasque(Pixels * *Fond, Pixels * *Image, int width, int height, int n);
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
/// Renvoie la taille de la composante connexe
/// </summary>
/// <param name="tab">Tableau</param>
/// <param name="copyTab">Copie du tableau</param>
/// <param name="height">Hauteur</param>
/// <param name="width">Largeur</param>
/// <param name="x">x(Start)</param>
/// <param name="y">y(Start)</param>
/// <returns>Taille de la CC</returns>
int tailleConnexe(Pixels** tab, Pixels** copyTab, int height, int width, int x, int y);
/// <summary>
/// Renvoie la plus grande composante connexe
/// </summary>
/// <param name="tab">Tableau de pixels</param>
/// <param name="height">Hauteur</param>
/// <param name="width">Largeur</param>
/// <returns>Plus grande CC</returns>
Pixels** plusGrandConnexe(Pixels** tab, int height, int width);
/// <summary>
/// Fonction appliquant tout les masques correspondant aux images passes en parametre
/// </summary>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="Images">Liste des images</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Resultat final comportant le fond + les sujets</returns>
Pixels** MultiMasque(Pixels** Mediane, Image* Images, int nb, Pixels** fond, int width, int height);
/// <summary>
/// La moyenne de n Image. Le sujet est transparent
/// </summary>
/// <param name="tabPixels">Liste des images sous formes de tableau de pixels</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Tableau de pixels comportant les sujets transparent</returns>
Pixels** MoyenneNimages(std::list<Pixels**> tabPixels, int width, int height);
/// <summary>
/// Le sujet deviens de plus en plus opaque
/// </summary>
/// <param name="tabPixels">Liste des images sous formes de tableau de pixels</param>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Tableau de pixels comportant les sujets de plus en plus opaque</returns>
Pixels** Fading_front(Image* Images, Pixels** Mediane,int nb ,int width, int height);

/// <summary>
/// Le sujet deviens de plus en plus transparent
/// </summary>
/// <param name="tabPixels">Liste des images sous formes de tableau de pixels</param>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <returns>Tableau de pixels comportant les sujets de plus en plus transparent</returns>
Pixels** Fading_back(Image* Images, Pixels** Mediane,int nb ,int width, int height);

/// <summary>
/// Fonction gerant la superposition des images
/// </summary>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="tabImage">Liste des images</param>
/// <param name="nb">Nombre d'images</param>
/// <param name="fond">Mediane sans le flou</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <param name="overmax">Nombre de superpositions</param>
/// <returns>Image sans les masques superposes</returns>
Pixels** overlap(Pixels** Mediane, Image* tabImage, int nb, Pixels** fond, int width, int height, int overmax);

/// <summary>
/// Permet d'ajouter une distance entre les sujets
/// </summary>
/// <param name="Mediane">Fond de l'image / Mediane</param>
/// <param name="tabImage">Liste des images</param>
/// <param name="nb">Nombre d'images</param>
/// <param name="fond">Fond sans le flou</param>
/// <param name="width">Largeur</param>
/// <param name="height">Hauteur</param>
/// <param name="distMin">Distance minimale entre 2 sujets</param>
/// <returns>Image avec les sujets qui respectent la distanciation sociale</returns>
Pixels** distance(Pixels** Mediane, Image* tabImage, int nb, Pixels** fond, int width, int height, int distMin);

/// <summary>
/// Initialise tableau boolean a false
/// </summary>
/// <param name="tabMasqueValide">Gere les masques les utiliser</param>
/// <param name="nb">Nombre de masques total</param>
void InitTabBool(bool* tabMasqueValide, int const& nb);


/// <summary>
/// Init a 0 un tableau de int
/// </summary>
/// <param name="tabNbOver">Tableau d'entiers</param>
/// <param name="nb">Nombre total</param>
void InittabNbOver(int* tabNbOver, int const& nb);
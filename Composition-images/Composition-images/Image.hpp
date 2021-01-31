#pragma once
#include "corona.h"
#include <string>
/// <summary>
/// Structure d'un pixel en RGBA
/// </summary>
struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};
/// <summary>
/// Structure d'un pixel en RGBA avec des valeurs float
/// </summary>
struct PixelsFloat {

	float red;
	float green;
	float blue;
	float alpha;
};
typedef unsigned char byte;

/// <summary>
/// Classe stockant l'image sous forme corona::image, son tableau de pixels, son nom et ses dimensions
/// </summary>
class Image {

private:
	/// <summary>
	/// Largeur
	/// </summary>
	int width;
	/// <summary>
	/// Hauteur
	/// </summary>
	int height;
	/// <summary>
	/// Tableau de pixels
	/// </summary>
	Pixels** tabPixels;
	/// <summary>
	/// Image sous forme de corona::image
	/// </summary>
	corona::Image* img;
	/// <summary>
	/// Nom de l'image (chemin complet)
	/// </summary>
	std::string name;
public:
	/// <summary>
	/// Constructeur par défaut
	/// </summary>
	Image();
	/// <summary>
	/// Constructeur pour initialiser l'image a une image noire
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="name"></param>
	Image(int width, int height,std::string name = "");
	/// <summary>
	/// Ouvre une image a partir de son nom
	/// </summary>
	/// <param name="name">Nom de l'image(Mettre le chemin)</param>
	Image(const char* name);
	~Image();
	/// <summary>
	/// Retourne la largeur
	/// </summary>
	/// <returns>width</returns>
	int getWidth() const;
	/// <summary>
	/// Retourne la hauteur
	/// </summary>
	/// <returns>height</returns>
	int getHeight() const;
	/// <summary>
	/// Retourne le tableau de pixels
	/// </summary>
	/// <returns>tabPixels</returns>
	Pixels** getTabPixels() const;
	/// <summary>
	/// Initialise un tableau avec comme valeur par défaut {0,0,0,255}
	/// </summary>
	/// <param name="largeur">Largeur</param>
	/// <param name="hauteur">Hauteur</param>
	/// <returns>Tableau initialiser</returns>
	Pixels** init(int largeur, int hauteur);
	/// <summary>
	/// Convertir l'image en tableau de pixels
	/// </summary>
	/// <param name="img">Image a convertir</param>
	/// <returns>Tableau de pixels correspondant a l'image</returns>
	Pixels** ImageToPixels(corona::Image* img);
	/// <summary>
	/// Transfert le tableau de pixels dans l'image
	/// </summary>
	/// <param name="pix">Tableau de pixels</param>
	/// <param name="img">Image recevant le tableau</param>
	void TabToPixels(Pixels** pix, corona::Image* img);
	/// <summary>
	/// Modifie le tableau de pixels actuel par celui passer en parametre
	/// </summary>
	/// <param name="tab">Tableau qui donnera ses valeurs au tableau de l'image</param>
	void setTabPixels(Pixels** tab);
	/// <summary>
	/// Change le nom de l'image
	/// </summary>
	/// <param name="name">Nom</param>
	void setName(std::string name);
	/// <summary>
	/// Sauvegarde l'image a partir de son nom
	/// </summary>
	void saveImg();
};
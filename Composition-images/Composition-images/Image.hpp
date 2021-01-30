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

class Image {

private:
	int width;
	int height;
	Pixels** tabPixels;
	corona::Image* img;
	std::string name;
public:
	Image();
	Image(int, int,std::string = "");
	Image(const char*);
	~Image();

	int getWidth() const;
	int getHeight() const;
	Pixels** getTabPixels() const;
	Pixels** init(int largeur, int hauteur);
	Pixels** ImageToPixels(corona::Image* img);
	void TabToPixels(Pixels** pix, corona::Image* img);
	void setTabPixels(Pixels**);
	void setName(std::string);
	void saveImg();
};
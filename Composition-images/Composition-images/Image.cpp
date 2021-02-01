#include "Image.hpp"			 	      	   	  

Image::Image() {
	width = 0;
	height = 0;
	name = "";
	tabPixels = init(width, height);
	corImg = corona::CreateImage(width, height, corona::PF_R8G8B8);
}
Image::Image(int width, int height, std::string name) {
	this->width = width;
	this->height = height;
	this->name = name;
	tabPixels = init(width, height);
	corImg = corona::CreateImage(width, height, corona::PF_R8G8B8);
}
Image::Image(const char* name) {
	corImg = corona::OpenImage(name, corona::PF_R8G8B8);
	width = corImg->getWidth();
	height = corImg->getHeight();
	this->name = name;
	tabPixels = ImageToPixels(corImg);
}
Image::Image(const Image& img) {
	width = img.getWidth();
	height = img.getHeight();
	name = img.getName();
	tabPixels = init(width, height);
	for (int x = 0; x < height; ++x) {
		for (int y = 0; y < width; ++y) {
			tabPixels[x][y] = img.getPix(x, y);
		}
	}
	corImg = img.corImg;
}
Image::~Image() {
	for (int x = 0; x < height; ++x) {
		delete[] tabPixels[x];
	}
	delete[] tabPixels;
}

Image::Image(const Image & im)
{
	this->width = im.width;
	this->height = im.height;
	this->name = im.name;
	this->tabPixels = im.tabPixels;
	this->img = im.img;
}


int Image::getWidth() const {
	return width;
}
int Image::getHeight() const {
	return height;
}
Pixels** Image::getTabPixels() {
	return tabPixels;
}
Pixels Image::getPix(int i, int j) const {
	return tabPixels[i][j];
}
std::string Image::getName() const{
	return name;
}
corona::Image* Image::getImg() const {
	return corImg;
}
Pixels** Image::init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [hauteur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = new Pixels[largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0};
		}
	}
	return tab;
}

Pixels** Image::ImageToPixels(corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* pixels = img->getPixels();
	Pixels** tab = init(width, height);
	int red, green, blue;
	byte* p;
	// we're guaranteed that the first eight bits of every pixel is red,
	// the next eight bits is green, and so on...
	p = (byte*)pixels;
	for (int x = 0; x < height; ++x) {
		for (int j = 0; j < width; ++j) {
			red = *p++;
			green = *p++;
			blue = *p++;
			tab[x][j] = { red,green,blue};
		}
	}
	return tab;
}

void Image::setTabPixels(Pixels** tab) {
	for (int x = 0; x < height; ++x) {
		for (int y = 0; y < width; ++y) {
			tabPixels[x][y] = tab[x][y];
		}
	}
	TabToPixels(tabPixels, corImg);


}

void Image::TabToPixels(Pixels** pix, corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* p = img->getPixels();
	Pixels tmp;
	byte* iter = (byte*)p;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			tmp = pix[i][j];
			*iter = tmp.red;
			*++iter;
			*iter = tmp.green;
			*++iter;
			*iter = tmp.blue;
			*++iter;
		}
	}
}
void Image::setName(std::string nom)
{
	name = nom;
}

void Image::saveImg() {
	corona::SaveImage(name.c_str(), corona::FileFormat::FF_PNG, corImg);
}

void Image::swap(Image& img) {
	std::swap(width, img.width);
	std::swap(height, img.height);
	std::swap(name, img.name);
	std::swap(tabPixels, img.tabPixels);
	std::swap(corImg, img.corImg);


}

Image& Image::operator=(const Image& img) {
	Image tmp(img);
	swap(tmp);
	return *this;
}


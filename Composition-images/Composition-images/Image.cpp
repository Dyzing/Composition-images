#include "Image.hpp"


Image::Image() {
	width = 1280;
	height = 720;
	name = "";
	tabPixels = init(width, height);
	img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
}
Image::Image(int width, int height, std::string name) {
	this->width = width;
	this->height = height;
	this->name = name;
	tabPixels = init(width, height);
	img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
}
Image::Image(const char* name) {
	img = corona::OpenImage(name, corona::PF_R8G8B8A8);
	width = img->getWidth();
	height = img->getHeight();
	name = "";
	tabPixels = ImageToPixels(img);
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
Pixels** Image::getTabPixels() const {
	return tabPixels;
}

Pixels** Image::init(int largeur, int hauteur) {
	Pixels** tab = new Pixels * [hauteur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = new Pixels[largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0,255 };
		}
	}
	return tab;
}

Pixels** Image::ImageToPixels(corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* pixels = img->getPixels();
	Pixels** tab = init(width, height);
	int red, green, blue, alpha;
	byte* p;
	// we're guaranteed that the first eight bits of every pixel is red,
	// the next eight bits is green, and so on...
	p = (byte*)pixels;
	for (int x = 0; x < height; ++x) {
		for (int j = 0; j < width; ++j) {
			red = *p++;
			green = *p++;
			blue = *p++;
			alpha = *p++;
			tab[x][j] = { red,green,blue,alpha };
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
	TabToPixels(tabPixels, img);


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
			*iter++;
			*iter = tmp.green;
			*iter++;
			*iter = tmp.blue;
			*iter++;
			*iter = tmp.alpha;
			*iter++;
		}
	}
}
void Image::setName(std::string nom)
{
	name = nom;
}

void Image::saveImg() {
	corona::SaveImage(name.c_str(), corona::FileFormat::FF_PNG, img);
}


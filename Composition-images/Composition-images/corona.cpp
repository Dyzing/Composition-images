#include <corona.h>
#include <iostream>


struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};
typedef unsigned char byte;
Pixels** init(int hauteur, int largeur) {
	Pixels** tab = new Pixels * [hauteur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = &tab2[i * largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0,255 };
		}
	}


	return tab;
}

void TabToPixels(Pixels** pix, corona::Image* img) {
	int width = img->getWidth();
	int height = img->getHeight();
	void* p = img->getPixels();
	Pixels tmp;
	byte* iter = (byte*)p;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
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
int main()
{
	corona::Image* image = corona::OpenImage("../Photos/part1.jpg", corona::PF_R8G8B8A8);
	if (!image) {
		// error!
	}

	int width = image->getWidth();
	int height = image->getHeight();
	void* pixels = image->getPixels();
	Pixels** tab = init(width, height);
	// we're guaranteed that the first eight bits of every pixel is red,
	// the next eight bits is green, and so on...
	
	byte* p = (byte*)pixels;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			int red = *p++;
			int green = *p++;
			int blue = *p++;
			int alpha = *p++;
			//tab[i][j] = { red,green,blue,alpha };
		}
	}
	

	corona::Image*  imagecop =	corona::CloneImage(image);
	TabToPixels(tab, imagecop);
	corona::SaveImage("../Photos/coucou.jpg", corona::FileFormat::FF_PNG, imagecop);

}
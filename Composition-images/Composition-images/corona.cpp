#include <corona.h>
#include <iostream>


struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};

Pixels** init(int hauteur, int largeur) {
	Pixels** tab = new Pixels * [hauteur];
	Pixels* tab2 = new Pixels[hauteur * largeur];
	for (int i = 0; i < hauteur; i++) {
		tab[i] = &tab2[i * largeur];

		for (int j = 0; j < largeur; j++) {
			tab[i][j] = { 0,0,0,0 };
		}
	}


	return tab;
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
	typedef unsigned char byte;
	byte* p = (byte*)pixels;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			int red = *p++;
			int green = *p++;
			int blue = *p++;
			int alpha = *p++;
			tab[i][j] = { red,green,blue,alpha };
		}
	}
	
	corona::Image*  imagecop =	corona::CloneImage(image);
	corona::SaveImage("../Photos/coucou.jpg", corona::FileFormat::FF_PNG, imagecop);

}
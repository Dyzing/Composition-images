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
			tab[i][j] = { 0,0,255,255 };
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
int main(int argc, char* argv[])
{
	corona::Image* image = corona::OpenImage("../Photos/part1.jpg", corona::PF_R8G8B8A8);
	if (!image) {
		// error!
	}

	int width = image->getWidth();
	int height = image->getHeight();
	void* pixels = image->getPixels();
	Pixels** tab1 = init(width, height);
	Pixels** tab2 = init(width, height);
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

	corona::Image* imagecop2 = corona::CloneImage(image);
	TabToPixels(tab2, imagecop2);

	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (j == height / 2)
			{
				tab1[i][j] = { 255,0,0,255 };
			}
		}
	}

	corona::Image* imagecop1 = corona::CloneImage(image);
	TabToPixels(tab1, imagecop1);

	Pixels** tab3 = init(width, height);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			tab3[i][j].red = tab2[i][j].red - tab1[i][j].red;
			tab3[i][j].green = tab2[i][j].green - tab1[i][j].green;
			tab3[i][j].blue = tab2[i][j].blue - tab1[i][j].blue;
			//tab3[i][j].alpha = tab2[i][j].alpha - tab1[i][j].alpha;
		}
	}

	corona::Image* imagecop3 = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(tab3, imagecop3);

	corona::SaveImage("../Photos/imagecop1.jpg", corona::FileFormat::FF_PNG, imagecop1);
	corona::SaveImage("../Photos/imagecop2.jpg", corona::FileFormat::FF_PNG, imagecop2);
	corona::SaveImage("../Photos/imagecop3.jpg", corona::FileFormat::FF_PNG, imagecop3);

}
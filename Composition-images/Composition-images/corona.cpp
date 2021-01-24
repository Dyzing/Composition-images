#include <corona.h>
#include <iostream>

int main()
{
	corona::Image* image = corona::OpenImage("../Photos/part1.jpg", corona::PF_R8G8B8A8);
	if (!image) {
		// error!
	}

	int width = image->getWidth();
	int height = image->getHeight();
	void* pixels = image->getPixels();
	// we're guaranteed that the first eight bits of every pixel is red,
	// the next eight bits is green, and so on...
	typedef unsigned char byte;
	byte* p = (byte*)pixels;
	for (int i = 0; i < width * height; ++i) {
		byte red = *p++;
		byte green = *p++;
		byte blue = *p++;
		byte alpha = *p++;
	}
	corona::Image*  imagecop =	corona::CloneImage(image);
	corona::SaveImage("../Photos/shrek.jpg", corona::FileFormat::FF_PNG, imagecop);

}
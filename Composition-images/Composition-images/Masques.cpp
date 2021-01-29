#include "Masques.hpp"
#include<string>


Pixels * *CreationMasque(Pixels * *Fond, Pixels * *Image, int width, int height, int n) {
	Image = FlouGaussien(Image, width, height);
	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tabFinal[i][j].red = std::abs(Fond[i][j].red - Image[i][j].red);
			tabFinal[i][j].green = std::abs(Fond[i][j].green - Image[i][j].green);
			tabFinal[i][j].blue = std::abs(Fond[i][j].blue - Image[i][j].blue);
		}
	}

	corona::Image* MasqueAppliquer = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	TabToPixels(tabFinal, MasqueAppliquer);
	std::string test = std::to_string(n);
	std::string test2 = "../Photos/Masque";
	test2 += test + ".jpg";
	char* cstr;
	cstr = &test2[0];

	corona::SaveImage(cstr, corona::FileFormat::FF_PNG, MasqueAppliquer);

	return tabFinal;
}

Pixels** AppliquerMasque(Pixels** Fond, Pixels** ImgBase, Pixels** Masque, int width, int height) {

	Pixels** tabFinal = init(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (!(Masque[i][j].red <= 12 && Masque[i][j].green <= 12 && Masque[i][j].blue <= 12)) {
				tabFinal[i][j].red = ImgBase[i][j].red;
				tabFinal[i][j].green = ImgBase[i][j].green;
				tabFinal[i][j].blue = ImgBase[i][j].blue;
			}
			else {
				tabFinal[i][j].red = Fond[i][j].red;
				tabFinal[i][j].green = Fond[i][j].green;
				tabFinal[i][j].blue = Fond[i][j].blue;
			}
			/*if (Masque[i][j].green > 15)
				tabFinal[i][j].green = ImgBase[i][j].green;
			else
				tabFinal[i][j].green = Fond[i][j].green;
			if (Masque[i][j].blue > 15)
				tabFinal[i][j].blue = ImgBase[i][j].blue;
			else
				tabFinal[i][j].blue = Fond[i][j].blue;*/

		}
	}
	return tabFinal;
}


Pixels** MultiMasque(Pixels** Mediane, std::list<Pixels**> tabPixels, int width, int height) {
	Pixels** masque;
	Pixels** resFinal = Mediane;
	corona::Image* img = corona::CreateImage(width, height, corona::PF_R8G8B8A8);
	int n = 0;
	for (Pixels** tab : tabPixels) {
		masque = CreationMasque(Mediane, tab, width, height, n);
		resFinal = AppliquerMasque(resFinal, tab, masque, width, height);
		n += 1;
	}

	return resFinal;
}
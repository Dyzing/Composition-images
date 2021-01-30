#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <cmath>
#include <algorithm>
/// <summary>
/// Structure d'un pixel en RGBA
/// </summary>
struct Pixels {

	int red;
	int green;
	int blue;
	int alpha;

};

struct PixelsFloat {

	float red;
	float green;
	float blue;
	float alpha;
};
typedef unsigned char byte;
/// <summary>
/// Test si l'option existe
/// </summary>
/// <param name="begin">Debut Arguments</param>
/// <param name="end">Fin Arguments</param>
/// <param name="option">Option a chercher</param>
/// <returns>Trouver ou non</returns>
bool cmdOptionExists(char** begin, char** end, const std::string& option);
/// <summary>
/// Recupere la valeur de l'option
/// </summary>
/// <param name="begin">Debut Arguments</param>
/// <param name="end">Fin Arguments</param>
/// <param name="option">Option a chercher</param>
/// <returns>Valeur de l'option</returns>
char* getCmdOption(char** begin, char** end, const std::string& option);

void CreateMedianes(std::list<Pixels**> tabPixels, corona::Image** tabImage);
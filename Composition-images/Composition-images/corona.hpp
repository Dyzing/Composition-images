#pragma once
#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <cmath>
#include <algorithm>
#include "Image.hpp"




inline bool operator==(Pixels p, Pixels p2){
	return p.red == p2.red && p.green == p2.green && p.blue == p2.blue && p.alpha == p2.alpha;
}
inline bool operator!=(Pixels p, Pixels p2) {
	return (p.red != p2.red || p.green != p2.green || p.blue != p2.blue || p.alpha != p2.alpha);
}
inline bool operator<(Pixels p, int x) {
	return (p.red < x && p.green < x && p.blue < x);
}
inline bool operator<=(Pixels p, int x) {
	return (p.red <= x && p.green <= x && p.blue <= x);
}
inline bool operator>(Pixels p, int x) {
	return (p.red > x&& p.green > x&& p.blue > x);
}
inline bool operator>=(Pixels p, int x) {
	return (p.red >= x && p.green >= x && p.blue >= x);
}

inline bool operator==(PixelsFloat p, PixelsFloat p2) {
	return p.red == p2.red && p.green == p2.green && p.blue == p2.blue && p.alpha == p2.alpha;
}
inline bool operator!=(PixelsFloat p, PixelsFloat p2) {
	return (p.red != p2.red || p.green != p2.green || p.blue != p2.blue || p.alpha != p2.alpha);
}
inline bool operator<(PixelsFloat p, int x) {
	return (p.red < x&& p.green < x&& p.blue < x);
}
inline bool operator<=(PixelsFloat p, int x) {
	return (p.red <= x && p.green <= x && p.blue <= x);
}
inline bool operator>(PixelsFloat p, int x) {
	return (p.red > x && p.green > x && p.blue > x);
}
inline bool operator>=(PixelsFloat p, int x) {
	return (p.red >= x && p.green >= x && p.blue >= x);
}

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
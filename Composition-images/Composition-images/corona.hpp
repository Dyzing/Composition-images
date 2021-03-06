#pragma once
#include <corona.h>
#include <iostream>
#include <stdio.h>
#include <list>
#include <cmath>
#include <algorithm>
#include "Image.hpp"



extern int tolerance;

inline bool operator==(Pixels p, Pixels p2){
	return p.red == p2.red && p.green == p2.green && p.blue == p2.blue;
}
inline bool operator!=(Pixels p, Pixels p2) {
	return (p.red != p2.red || p.green != p2.green || p.blue != p2.blue);
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
	return p.red == p2.red && p.green == p2.green && p.blue == p2.blue;
}
inline bool operator!=(PixelsFloat p, PixelsFloat p2) {
	return (p.red != p2.red || p.green != p2.green || p.blue != p2.blue);
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
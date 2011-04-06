#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "PerlinNoise.hpp"

terra::PerlinNoise::PerlinNoise(unsigned int NewSeed){
	Seed = NewSeed;
}

double terra::PerlinNoise::Interpolate(double A, double B, double X){
	return (1.-X)*A+X*B;
	//return ((A-B)*cos(3.14159265358979*X)+A+B)/2.;
}

double terra::PerlinNoise::GetSmooth(int X){
	double Center = Rand(X);
	double Sides = (Rand(X-1)+Rand(X+1))/2.;
	return (2.*Center+Sides)/3.;
}

double terra::PerlinNoise::GetSmooth(int X, int Y){
	double Center = Rand(X, Y);
	double Sides1 = (Rand(X-1, Y)+Rand(X+1, Y))/2.;
	double Sides2 = (Rand(X, Y-1)+Rand(X, Y+1))/2.;
	double Sides = (Sides1+Sides2)/2.;
	double Corners1 = (Rand(X-1, Y-1)+Rand(X+1, Y-1))/2.;
	double Corners2 = (Rand(X-1, Y+1)+Rand(X+1, Y+1))/2.;
	double Corners = (Corners1+Corners2)/2.;
	return (3.*Center+2.*Sides+Corners)/6.;
}

double terra::PerlinNoise::Noise1D(double X, unsigned int Layers){
	if (Layers == 0)
		return 0.;
	int TruncatedX = static_cast<int>(X);
	double Left = GetSmooth(TruncatedX);
	double Right = GetSmooth(TruncatedX+1);
	double Next = 0.5*Noise1D(2.*X, Layers-1);
	if (Layers > 1)
		return (Interpolate(Left, Right, X-TruncatedX)+Next)/1.5;
	else
		return Interpolate(Left, Right, X-TruncatedX);
}

double terra::PerlinNoise::Noise2D(double X, double Y, unsigned int Layers){
	if (Layers == 0)
		return 0.;
	int TruncatedX = static_cast<int>(X);
	int TruncatedY = static_cast<int>(Y);
	double TopLeft = GetSmooth(TruncatedX, TruncatedY);
	double TopRight = GetSmooth(TruncatedX+1, TruncatedY);
	double BottomLeft = GetSmooth(TruncatedX, TruncatedY+1);
	double BottomRight = GetSmooth(TruncatedX+1, TruncatedY+1);
	double Top = Interpolate(TopLeft, TopRight, X-TruncatedX);
	double Bottom = Interpolate(BottomLeft, BottomRight, X-TruncatedX);
	double Next = 0.5*Noise2D(2.*X, 2.*Y, Layers-1);
	if (Layers > 1)
		return (Interpolate(Top, Bottom, Y-TruncatedY)+Next)/1.5;
	else
		return Interpolate(Top, Bottom, Y-TruncatedY);
}

double terra::PerlinNoise::Rand(int X){
	uint32_t Hash = 0;
	for (int i = 0; i < 4; ++i){
		Hash += (X & (0xFF << 8*i)) >> 8*i;
		Hash += Hash << 10;
		Hash ^= Hash >> 6;
	}
	for (int i = 0; i < 4; ++i){
		Hash += (Seed & (0xFF << 8*i)) >> 8*i;
		Hash += Hash << 10;
		Hash ^= Hash >> 6;
	}
	Hash += Hash << 3;
	Hash ^= Hash >> 11;
	Hash += Hash << 15;
	return Hash/4294967296.;
}

double terra::PerlinNoise::Rand(int X, int Y){
	uint32_t Hash = 0;
	for (int i = 0; i < 4; ++i){
		Hash += (X & (0xFF << 8*i)) >> 8*i;
		Hash += Hash << 10;
		Hash ^= Hash >> 6;
	}
	for (int i = 0; i < 4; ++i){
		Hash += (Y & (0xFF << 8*i)) >> 8*i;
		Hash += Hash << 10;
		Hash ^= Hash >> 6;
	}
	for (int i = 0; i < 4; ++i){
		Hash += (Seed & (0xFF << 8*i)) >> 8*i;
		Hash += Hash << 10;
		Hash ^= Hash >> 6;
	}
	Hash += Hash << 3;
	Hash ^= Hash >> 11;
	Hash += Hash << 15;
	return Hash/4294967296.;
}

void terra::PerlinNoise::Reseed(unsigned int NewSeed){
	Seed = NewSeed;
}

terra::PerlinNoise::~PerlinNoise(){
}

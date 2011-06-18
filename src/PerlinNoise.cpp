#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "PerlinNoise.hpp"

terra::PerlinNoise::PerlinNoise(unsigned int NewSeed){
	Seed = NewSeed;
}

double terra::PerlinNoise::Interpolate(double A, double B, double X){
	return (1.-X)*A+X*B;
}

double terra::PerlinNoise::GetSmooth(int X){
	// Do some averaging of values
	double Center = Rand(X);
	double Sides = (Rand(X-1)+Rand(X+1))/2.;
	return (2.*Center+Sides)/3.;
}

double terra::PerlinNoise::GetSmooth(int X, int Y){
	// Do some 2D averaging of values
	double Center = Rand(X, Y);
	double Sides1 = (Rand(X-1, Y)+Rand(X+1, Y))/2.;
	double Sides2 = (Rand(X, Y-1)+Rand(X, Y+1))/2.;
	double Sides = (Sides1+Sides2)/2.;
	double Corners1 = (Rand(X-1, Y-1)+Rand(X+1, Y-1))/2.;
	double Corners2 = (Rand(X-1, Y+1)+Rand(X+1, Y+1))/2.;
	double Corners = (Corners1+Corners2)/2.;
	return (3.*Center+2.*Sides+Corners)/6.;
}

double terra::PerlinNoise::Noise1D(double X, unsigned int Octaves, double Persistence, double ScalingX){
	// Exit if there are no octaves to process
	if (Octaves == 0)
		return 0.;

	// Fancy math time!
	int TruncatedX = static_cast<int>(X);
	double Left = GetSmooth(TruncatedX);
	double Right = GetSmooth(TruncatedX+1);

	// Handle multiple octaves
	if (Octaves > 1)
		return (Interpolate(Left, Right, X-TruncatedX)+Persistence*Noise1D(ScalingX*X, Octaves-1, Persistence))/(1.+Persistence);
	else
		return Interpolate(Left, Right, X-TruncatedX);
}

double terra::PerlinNoise::Noise2D(double X, double Y, unsigned int Octaves, double Persistence, double ScalingX, double ScalingY){
	// Exit if there are no octaves to process
	if (Octaves == 0)
		return 0.;

	// Fancy math time! Now in 2D!
	int TruncatedX = static_cast<int>(X);
	int TruncatedY = static_cast<int>(Y);
	double TopLeft = GetSmooth(TruncatedX, TruncatedY);
	double TopRight = GetSmooth(TruncatedX+1, TruncatedY);
	double BottomLeft = GetSmooth(TruncatedX, TruncatedY+1);
	double BottomRight = GetSmooth(TruncatedX+1, TruncatedY+1);
	double Top = Interpolate(TopLeft, TopRight, X-TruncatedX);
	double Bottom = Interpolate(BottomLeft, BottomRight, X-TruncatedX);

	// Handle multiple octaves
	if (Octaves > 1)
		return (Interpolate(Top, Bottom, Y-TruncatedY)+Persistence*Noise2D(ScalingX*X, ScalingY*Y, Octaves-1, Persistence))/(1.+Persistence);
	else
		return Interpolate(Top, Bottom, Y-TruncatedY);
}

double terra::PerlinNoise::Rand(int X){
	// This is just a hash function. Perlin Noise doesn't actually use random numbers, it uses hashes.
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
	// This is just a hash function. Perlin Noise doesn't actually use random numbers, it uses hashes.
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

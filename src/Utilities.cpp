#include <cstdint>
#include <iostream>
#include <map>
#include "Utilities.hpp"

std::map<std::string, sf::SoundBuffer> SoundMap;
std::map<std::string, sf::Image> TextureMap;

sf::SoundBuffer &terra::GetSound(std::string SoundName){
	if (SoundMap.find(SoundName) == SoundMap.end()){
		sf::SoundBuffer Temp;
		if (!Temp.LoadFromFile(SoundName))
			std::cerr << "Unable to load sound " << SoundName << '\n';
		SoundMap.insert(std::pair<std::string, sf::SoundBuffer>(SoundName, Temp));
	}
	return SoundMap.find(SoundName)->second;
}

sf::Image &terra::GetTexture(std::string TextureName){
	if (TextureMap.find(TextureName) == TextureMap.end()){
		sf::Image Temp;
		if (!Temp.LoadFromFile(TextureName))
			std::cerr << "Unable to load texture " << TextureName << '\n';
		TextureMap.insert(std::pair<std::string, sf::Image>(TextureName, Temp));
	}
	return TextureMap.find(TextureName)->second;
}

bool terra::IsBigEndian(){
	int16_t One = 1;
	return (reinterpret_cast<char*>(&One)[0] == 0);
}

bool terra::IsColliding(sf::Shape A, sf::Shape B){
	if (A.GetPointsCount() < 2 || B.GetPointsCount() < 2)
		return false;
	for (unsigned int i = 0; i < A.GetPointsCount(); ++i){
		sf::Vector2f Vector = A.GetPointPosition((i+1)%A.GetPointsCount())-A.GetPointPosition(i);
		sf::Vector2f Axis(Vector.y, -Vector.x);
		float AMin = Axis.x*A.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float AMax = AMin;
		float BMin = Axis.x*B.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float BMax = BMin;
		for (unsigned int j = 1; j < A.GetPointsCount(); ++j){
			float Pos = Axis.x*A.GetPointPosition(j).x+Axis.y*A.GetPointPosition(j).y;
			AMin = AMin < Pos ? AMin : Pos;
			AMax = AMax > Pos ? AMax : Pos;
		}
		for (unsigned int j = 1; j < B.GetPointsCount(); ++j){
			float Pos = Axis.x*B.GetPointPosition(j).x+Axis.y*B.GetPointPosition(j).y;
			BMin = BMin < Pos ? BMin : Pos;
			BMax = BMax > Pos ? BMax : Pos;
		}
		if (AMin > BMax || AMax < BMin)
			return false;
	}
	for (unsigned int i = 0; i < B.GetPointsCount(); ++i){
		sf::Vector2f Vector = B.GetPointPosition((i+1)%B.GetPointsCount())-B.GetPointPosition(i);
		sf::Vector2f Axis(Vector.y, -Vector.x);
		float AMin = Axis.x*A.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float AMax = AMin;
		float BMin = Axis.x*B.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float BMax = BMin;
		for (unsigned int j = 1; j < A.GetPointsCount(); ++j){
			float Pos = Axis.x*A.GetPointPosition(j).x+Axis.y*A.GetPointPosition(j).y;
			AMin = AMin < Pos ? AMin : Pos;
			AMax = AMax > Pos ? AMax : Pos;
		}
		for (unsigned int j = 1; j < B.GetPointsCount(); ++j){
			float Pos = Axis.x*B.GetPointPosition(j).x+Axis.y*B.GetPointPosition(j).y;
			BMin = BMin < Pos ? BMin : Pos;
			BMax = BMax > Pos ? BMax : Pos;
		}
		if (AMin > BMax || AMax < BMin)
			return false;
	}
	return true;
}

void terra::SwapEndianness(char *Bytes, unsigned long NumBytes){
	if (!Bytes)
		return;
	char *Temp = new char [NumBytes];
	for (unsigned long i = 0; i < NumBytes; ++i)
		Temp[i] = Bytes[i];
	for (unsigned long i = 0; i < NumBytes; ++i)
		Bytes[i] = Temp[NumBytes-(i+1)];
	delete [] Temp;
}

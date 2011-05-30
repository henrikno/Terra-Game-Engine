#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include "Engine.hpp"
#include "Utilities.hpp"

std::string CurrentMusic;
std::map<std::string, std::shared_ptr<sf::Music>> MusicMap;
std::map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundMap;
std::map<std::string, std::shared_ptr<sf::Image>> TextureMap;

std::list<unsigned int> terra::DetectConcavePoints(sf::Shape Shape){
	std::list<unsigned int> PointList;
	for (unsigned int i = 0; i < Shape.GetPointsCount(); ++i){
		sf::Vector2f LeftPoint = Shape.GetPointPosition(i > 0 ? i-1 : Shape.GetPointsCount()-1);
		sf::Vector2f Point = Shape.GetPointPosition(i);
		sf::Vector2f RightPoint = Shape.GetPointPosition(i < Shape.GetPointsCount()-1 ? i+1 : 0);
		LeftPoint -= Point;
		RightPoint -= Point;
		double Length = sqrt(Point.x*Point.x+Point.y*Point.y);
		double CosineTheta = Point.y/Length;
		double SineTheta = Point.x/Length;
		LeftPoint = sf::Vector2f(LeftPoint.x*CosineTheta-LeftPoint.y*SineTheta, LeftPoint.y*CosineTheta+LeftPoint.x*SineTheta);
		RightPoint = sf::Vector2f(RightPoint.x*CosineTheta-RightPoint.y*SineTheta, RightPoint.y*CosineTheta+RightPoint.x*SineTheta);
		double LeftSlope = (LeftPoint.y-Point.y)/LeftPoint.x;
		double RightSlope = (RightPoint.y-Point.y)/RightPoint.x;
		double Curvature = 2.*(LeftSlope-RightSlope)/(LeftPoint.x-RightPoint.x);
		if (Curvature > 0.)
			PointList.push_back(i);
	}
	return PointList;
}

std::shared_ptr<sf::SoundBuffer> terra::GetSound(std::string SoundName){
	if (SoundMap.find(SoundName) == SoundMap.end()){
		std::shared_ptr<sf::SoundBuffer> Temp(new sf::SoundBuffer);
		if (!Temp->LoadFromFile(SoundName))
			terra::Engine::Get().Error(std::string("Unable to load sound ") + SoundName + '\n');
		SoundMap.insert(std::pair<std::string, std::shared_ptr<sf::SoundBuffer>>(SoundName, Temp));
	}
	return SoundMap.find(SoundName)->second;
}

std::shared_ptr<sf::Image> terra::GetTexture(std::string TextureName){
	if (TextureMap.find(TextureName) == TextureMap.end()){
		std::shared_ptr<sf::Image> Temp(new sf::Image);
		if (!Temp->LoadFromFile(TextureName))
			terra::Engine::Get().Error(std::string("Unable to load texture ") + TextureName + '\n');
		TextureMap.insert(std::pair<std::string, std::shared_ptr<sf::Image>>(TextureName, Temp));
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

void terra::PauseMusic(){
	if (CurrentMusic.size())
		MusicMap.find(CurrentMusic)->second->Pause();
	CurrentMusic = "";
}

void terra::PlayMusic(std::string MusicName, bool Loop, bool Pause){
	if (Pause)
		terra::PauseMusic();
	else
		terra::StopMusic();
	if (MusicMap.find(MusicName) == MusicMap.end()){
		std::shared_ptr<sf::Music> Temp(new sf::Music);
		if (!Temp->OpenFromFile(MusicName)){
			terra::Engine::Get().Error(std::string("Unable to load music ") + MusicName + '\n');
			return;
		}
		MusicMap.insert(std::pair<std::string, std::shared_ptr<sf::Music>>(MusicName, Temp));
	}
	MusicMap.find(MusicName)->second->SetLoop(Loop);
	MusicMap.find(MusicName)->second->Play();
	CurrentMusic = MusicName;
}

void terra::StopMusic(){
	if (CurrentMusic.size())
		MusicMap.find(CurrentMusic)->second->Stop();
	CurrentMusic = "";
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

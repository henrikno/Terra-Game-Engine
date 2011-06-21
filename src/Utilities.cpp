#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <iterator>
#include "Engine.hpp"
#include "Utilities.hpp"

std::string CurrentMusic;
std::map<std::string, std::shared_ptr<sf::Music>> MusicMap;
std::map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundMap;
std::map<std::string, std::shared_ptr<sf::Image>> TextureMap;

std::list<unsigned int> terra::DetectConcavePoints(sf::Shape Shape){
	// Calculate the center of the shape from its points
	std::list<unsigned int> PointList;
	double CenterX = 0.;
	double CenterY = 0.;
	for (unsigned int i = 0; i < Shape.GetPointsCount(); ++i){
		CenterX += Shape.GetPointPosition(i).x;
		CenterY += Shape.GetPointPosition(i).y;
	}
	CenterX /= Shape.GetPointsCount();
	CenterY /= Shape.GetPointsCount();

	// Go through each point to see which are concave
	for (unsigned int i = 0; i < Shape.GetPointsCount(); ++i){
		// Grab the point and its neighbors
		sf::Vector2f LeftPoint = Shape.GetPointPosition(i > 0 ? i-1 : Shape.GetPointsCount()-1);
		sf::Vector2f Point = Shape.GetPointPosition(i);
		sf::Vector2f RightPoint = Shape.GetPointPosition(i < Shape.GetPointsCount()-1 ? i+1 : 0);

		// Translate so the shape's center is at the origin
		LeftPoint -= sf::Vector2f(CenterX, CenterY);
		Point -= sf::Vector2f(CenterX, CenterY);
		RightPoint -= sf::Vector2f(CenterX, CenterY);

		// Find the length from the center to the current point
		double Length = sqrt(Point.x*Point.x+Point.y*Point.y);

		// Cheat at trigonometry to rotate the points so the current point is vertical
		double CosineTheta = Point.y/Length;
		double SineTheta = Point.x/Length;
		double YLeft = LeftPoint.y*CosineTheta+LeftPoint.x*SineTheta;
		double YMiddle = Length;
		double YRight = RightPoint.y*CosineTheta+RightPoint.x*SineTheta;

		// Check if the point is concave and add it to the list
		if (YLeft > YMiddle || YRight > YMiddle)
			PointList.push_back(i);
	}
	return PointList;
}

std::shared_ptr<sf::SoundBuffer> terra::GetSound(std::string SoundName){
	// Standard Resource Loader
	if (SoundMap.find(SoundName) == SoundMap.end()){
		std::shared_ptr<sf::SoundBuffer> Temp(new sf::SoundBuffer);
		if (!Temp->LoadFromFile(SoundName))
			terra::Engine::Get().Error(std::string("Unable to load sound ") + SoundName + '\n');
		SoundMap.insert(std::pair<std::string, std::shared_ptr<sf::SoundBuffer>>(SoundName, Temp));
	}
	return SoundMap.find(SoundName)->second;
}

std::shared_ptr<sf::Image> terra::GetTexture(std::string TextureName){
	// Standard Resource Loader
	if (TextureMap.find(TextureName) == TextureMap.end()){
		std::shared_ptr<sf::Image> Temp(new sf::Image);
		if (!Temp->LoadFromFile(TextureName))
			terra::Engine::Get().Error(std::string("Unable to load texture ") + TextureName + '\n');
		TextureMap.insert(std::pair<std::string, std::shared_ptr<sf::Image>>(TextureName, Temp));
	}
	return TextureMap.find(TextureName)->second;
}

bool terra::IsBigEndian(){
	// I cheated. So sue me.
	int16_t One = 1;
	return (reinterpret_cast<char*>(&One)[0] == 0);
}

bool terra::IsColliding(sf::Shape A, sf::Shape B){
	// Separating Axis Theorem is fun
	// Abort if the shapes don't have enough points
	if (A.GetPointsCount() < 2 || B.GetPointsCount() < 2)
		return false;

	// Go through all of shape A's points
	for (unsigned int i = 0; i < A.GetPointsCount(); ++i){
		// Calculate the current axis
		sf::Vector2f Vector = A.GetPointPosition((i+1)%A.GetPointsCount())-A.GetPointPosition(i);
		sf::Vector2f Axis(Vector.y, -Vector.x);

		// Initialize the minimum and maximum values
		float AMin = Axis.x*A.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float AMax = AMin;
		float BMin = Axis.x*B.GetPointPosition(0).x+Axis.y*A.GetPointPosition(0).y;
		float BMax = BMin;

		// Go through all of the points in shape A to recalculate the minimum and maximum
		for (unsigned int j = 1; j < A.GetPointsCount(); ++j){
			float Pos = Axis.x*A.GetPointPosition(j).x+Axis.y*A.GetPointPosition(j).y;
			AMin = AMin < Pos ? AMin : Pos;
			AMax = AMax > Pos ? AMax : Pos;
		}

		// Do the same thing for shape B
		for (unsigned int j = 1; j < B.GetPointsCount(); ++j){
			float Pos = Axis.x*B.GetPointPosition(j).x+Axis.y*B.GetPointPosition(j).y;
			BMin = BMin < Pos ? BMin : Pos;
			BMax = BMax > Pos ? BMax : Pos;
		}

		// Check for freedom from collisions
		if (AMin > BMax || AMax < BMin)
			return false;
	}

	// Do that whole mess again for B's axes
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

	// Couldn't find an axis with no collision? Then there is a collision.
	return true;
}

void terra::PauseMusic(){
	// Yay hidden stuff
	if (CurrentMusic.size())
		MusicMap.find(CurrentMusic)->second->Pause();
	CurrentMusic = "";
}

void terra::PlayMusic(std::string MusicName, bool Loop, bool Pause){
	// Yay more hidden stuff
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

std::vector<char> terra::ReadFile(std::string Filename){
	// Open the file
	std::ifstream FileStream(Filename);

	// Copy file contents to a vector
	FileStream >> std::noskipws;
	std::vector<char> String;
	std::copy(std::istream_iterator<char>(FileStream), std::istream_iterator<char>(), std::back_inserter(String));

	// Abort if an error occurred
	if (!FileStream.good()){
		terra::Engine::Get().Error(std::string("Unexpected error occured when reading \"") + Filename + "\"\n");
		FileStream.close();
		String.resize(0);
		String.push_back(0);
		return String;
	}

	FileStream.close();
	return String;
}

void terra::StopMusic(){
	// EVEN MORE HIDDEN STUFF! ISN'T THIS AMAZING?!
	if (CurrentMusic.size())
		MusicMap.find(CurrentMusic)->second->Stop();
	CurrentMusic = "";
}

void terra::SwapEndianness(char *Bytes, unsigned long NumBytes){
	// Can't mess with NULL
	if (!Bytes)
		return;

	// Create a temp and copy the array into it
	char *Temp = new char [NumBytes];
	for (unsigned long i = 0; i < NumBytes; ++i)
		Temp[i] = Bytes[i];

	// Copy the temp into the array backwards
	for (unsigned long i = 0; i < NumBytes; ++i)
		Bytes[i] = Temp[NumBytes-(i+1)];

	// Don't forget to cleanup the temp
	delete [] Temp;
}

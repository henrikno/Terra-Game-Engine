#include "Item.hpp"

terra::Item::Item(sf::Vector2f InitialPosition, sf::Vector2<unsigned int> InitialSize){
	Position = InitialPosition;
	Size = InitialSize;
}

const sf::Vector2f &terra::Item::GetPosition() const{
	return Position;
}

const sf::Vector2<unsigned int> &terra::Item::GetSize() const{
	return Size;
}

void terra::Item::SetPosition(const sf::Vector2f &NewPosition){
	Position = NewPosition;
}

void terra::Item::SetSize(const sf::Vector2<unsigned int> &NewSize){
	Size = NewSize;
}

terra::Item::~Item(){
}

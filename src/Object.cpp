#include "Object.hpp"

terra::Object::Object(terra::OgmoObject ObjectData) : terra::Item(ObjectData.Position, ObjectData.Size){
	Name = ObjectData.Name;
}

const terra::Item::ItemType terra::Object::GetItemType() const{
	return terra::Item::Object;
}

const std::string &terra::Object::GetName() const{
	return Name;
}

terra::Object::~Object(){
}

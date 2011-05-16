#include "Object.hpp"

terra::Object::Object(std::string ObjectName){
	Name = ObjectName;
}

const terra::Item::ItemType terra::Object::GetItemType() const{
	return terra::Item::Object;
}

const std::string &terra::Object::GetName() const{
	return Name;
}

terra::Object::~Object(){
}

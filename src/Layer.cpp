#include "Layer.hpp"

terra::Layer::Layer(terra::Item::ItemType StoredType){
	StoredItem = StoredType;
}

void terra::Layer::AddItem(std::shared_ptr<Item> NewItem){
	if (!NewItem || NewItem->GetItemType() != GetStoredType())
		return;
	Items.push_back(NewItem);
}

std::list<std::shared_ptr<terra::Item>>::iterator terra::Layer::Begin(){
	return Items.begin();
}

void terra::Layer::Clear(){
	Items.clear();
}

std::list<std::shared_ptr<terra::Item>>::iterator terra::Layer::End(){
	return Items.end();
}

const terra::Item::ItemType terra::Layer::GetStoredType() const{
	return StoredItem;
}

void terra::Layer::RemoveItem(std::list<std::shared_ptr<Item>>::iterator ItemIterator){
	Items.erase(ItemIterator);
}

terra::Layer::~Layer(){
}

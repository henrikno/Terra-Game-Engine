#include "Tile.hpp"

terra::Tile::Tile(OgmoTile TileData) : terra::Item(TileData.Position, TileData.TileSize){
	Tileset = TileData.Tileset;
	TilePosition = TileData.TilePosition;
}

const terra::Item::ItemType terra::Tile::GetItemType() const{
	return terra::Item::Tile;
}

const sf::Vector2<unsigned int> terra::Tile::GetTilePosition() const{
	return TilePosition;
}

const std::string terra::Tile::GetTileset() const{
	return Tileset;
}

void terra::Tile::OnEvent(sf::Event Event){
}

void terra::Tile::OnFrame(){
}

void terra::Tile::OnRender(sf::RenderTarget &Target){
	if (!GetTexture(Tileset))
		return;
	sf::Sprite RenderMe(*GetTexture(Tileset));
	RenderMe.SetSubRect(sf::IntRect(sf::Vector2<int>(TilePosition), sf::Vector2<int>(GetSize())));
	RenderMe.SetPosition(GetPosition());
	Target.Draw(RenderMe);
}

terra::Tile::~Tile(){
}

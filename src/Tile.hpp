#ifndef TERRA_TILE_HPP
#define TERRA_TILE_HPP

#include "Item.hpp"
#include "OgmoTile.hpp"
#include "Utilities.hpp"

namespace terra{
	/*!
	 * \brief A Tile
	 *
	 * A tile.
	 */
	class Tile : public Item{
		private:
			std::string Tileset;
			sf::Vector2<unsigned int> TilePosition;
		public:
			/*!
			 * \param TileData Information on the tile that is being created
			 *
			 * Create a new tile from received data.
			 */
			Tile(OgmoTile TileData);

			/*!
			 * \return The type of the item
			 *
			 * Retrieve the type of the item.
			 */
			const ItemType GetItemType() const;

			/*!
			 * \return The position of the tile in the tileset
			 *
			 * Retrieve the position of the tile in the tileset.
			 */
			const sf::Vector2<unsigned int> GetTilePosition() const;

			/*!
			 * \return The tileset being used by the tile
			 *
			 * Retrieve the tileset being used by the tile.
			 */
			const std::string GetTileset() const;

			/*!
			 * \param Event The event to be processed
			 *
			 * Handle a single event.
			 */
			void OnEvent(sf::Event Event);

			/*!
			 * Do frame by frame updates.
			 */
			void OnFrame();

			/*!
			 * \param Target The target to be rendered to
			 *
			 * Render the item onto a target.
			 */
			void OnRender(sf::RenderTarget &Target);

			/*!
			 * Destroy the tile.
			 */
			~Tile();
	};
}

#endif

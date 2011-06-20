#ifndef TERRA_OGMOTILE_HPP
#define TERRA_OGMOTILE_HPP

#include <SFML/System.hpp>
#include <string>

namespace terra{
	/*!
	 * \brief Ogmo Tile
	 *
	 * A structure containing information on tiles that have been extracted from the level files.
	 */
	struct OgmoTile{
		/*!
		 * The filename of the tileset that this tile is from.
		 */
		std::string Tileset;

		/*!
		 * The position of the tile in the tileset.
		 */
		sf::Vector2<unsigned int> TilePosition;

		/*!
		 * The size of the tile in the tileset.
		 */
		sf::Vector2<unsigned int> TileSize;

		/*!
		 * The position of the tile in the world.
		 */
		sf::Vector2f Position;
	};
}

#endif

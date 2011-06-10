#ifndef TERRA_OGMOTILESET_HPP
#define TERRA_OGMOTILESET_HPP

#include <string>

namespace terra{
	/*!
	 * \brief Ogmo Tileset
	 *
	 * A structure containing information on tilesets that has been extracted from the level files.
	 */
	struct OgmoTileset{
		std::string Image;
		unsigned int TileWidth;
		unsigned int TileHeight;
	};
}

#endif

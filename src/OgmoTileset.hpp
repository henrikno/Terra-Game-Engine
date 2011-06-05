#ifndef TERRA_OGMOTILESET_HPP
#define TERRA_OGMOTILESET_HPP

#include <string>

namespace terra{
	/*!
	 * \brief An Ogmo Tileset
	 *
	 * An Ogmo Tileset with a filename.
	 */
	struct OgmoTileset{
		/*!
		 * The filename of the tileset
		 */
		std::string Filename;
	};
}

#endif

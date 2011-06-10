#ifndef TERRA_OGMOTILELAYER_HPP
#define TERRA_OGMOTILELAYER_HPP

namespace terra{
	/*!
	 * \brief Ogmo Tile Layers
	 *
	 * A structure containing information on tile layers that has been extracted from the level files.
	 */
	struct OgmoTileLayer{
		/*!
		 * Does the layer have multiple tilesets?
		 */
		bool MultipleTilesets;

		/*!
		 * Does the layer use a custom tile size?
		 */
		bool ExportTileSize;

		/*!
		 * Should the layer use IDs instead of tile positions?
		 */
		bool ExportTileIDs;
	};
}

#endif

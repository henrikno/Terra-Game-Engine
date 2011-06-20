#ifndef TERRA_OGMOOBJECT_HPP
#define TERRA_OGMOOBJECT_HPP

#include <SFML/System.hpp>
#include <string>

namespace terra{
	/*!
	 * \brief Ogmo Object
	 *
	 * A structure containing information on objects that have been extracted from the level files.
	 */
	struct OgmoObject{
		/*!
		 * The name of the object being loaded.
		 */
		std::string Name;

		/*!
		 * Is the object resizable on the x axis?
		 */
		bool ResizableX;

		/*!
		 * Is the object resizable on the y axis?
		 */
		bool ResizableY;

		/*!
		 * The position of the object.
		 */
		sf::Vector2f Position;

		/*!
		 * The size of the object.
		 */
		sf::Vector2<unsigned int> Size;
	};
}

#endif

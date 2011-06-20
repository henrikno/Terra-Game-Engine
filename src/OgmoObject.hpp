#ifndef TERRA_OGMOOBJECT_HPP
#define TERRA_OGMOOBJECT_HPP

#include <SFML/System.hpp>
#include <map>
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
		 * The values of the object being loaded.
		 */
		std::map<std::string, std::string> Values;

		/*!
		 * The types of the values in the object.
		 */
		std::map<std::string, std::string> ValueTypes;

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

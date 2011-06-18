#ifndef TERRA_OGMOOBJECT_HPP
#define TERRA_OGMOOBJECT_HPP

#include <SFML/System.hpp>
#include <string>

namespace terra{
	struct OgmoObject{
		std::string Name;
		bool ResizableX;
		bool ResizableY;
		sf::Vector2f Position;
		sf::Vector2<unsigned int> Size;
	};
}

#endif

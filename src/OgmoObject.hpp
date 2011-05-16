#ifndef TERRA_OGMOOBJECT_HPP
#define TERRA_OGMOOBJECT_HPP

#include <map>
#include <string>

namespace terra{
	/*!
	 * \brief An Ogmo Object
	 *
	 * An Ogmo Object with various attributes and value.
	 */
	struct OgmoObject{
		/*!
		 * The attributes and values of the Ogmo Object.
		 */
		std::map<std::string, std::string> Attributes;
	};
}

#endif

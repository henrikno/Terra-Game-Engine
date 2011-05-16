#ifndef TERRA_OBJECT_HPP
#define TERRA_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Item.hpp"

namespace terra{
	/*!
	 * \brief An Ogmo Object
	 *
	 * An Ogmo Object. Anything that has its own behavior is an object.
	 */
	class Object : public Item{
		private:
			std::string Name;
		public:
			/*!
			 * \param ObjectName The name of the object
			 *
			 * Create a new object.
			 */
			Object(std::string ObjectName);

			/*!
			 * \return The type of the item
			 *
			 * Retrieve the type of the item.
			 */
			const Item::ItemType GetItemType() const;

			/*!
			 * \return The name of the object
			 *
			 * Retrieve the name of the object.
			 */
			const std::string &GetName() const;

			/*!
			 * Destroy the object.
			 */
			~Object();
	};
}

#endif

#ifndef TERRA_ITEM_HPP
#define TERRA_ITEM_HPP

#include <SFML/Graphics.hpp>

namespace terra{
	/*!
	 * \brief An Ogmo Item
	 *
	 * An Ogmo Item. It can be a grid, a tile, or an object.
	 */
	class Item{
		public:
			/*!
			 * An enumeration of item types.
			 */
			enum ItemType{
				Object
			};

			/*!
			 * Create a new item.
			 */
			Item();

			/*!
			 * \return The type of the item
			 *
			 * Retrieve the type of the item.
			 */
			virtual const ItemType GetItemType() const = 0;

			/*!
			 * \param Event The event to be processed
			 *
			 * Handle a single event.
			 */
			virtual void OnEvent(sf::Event Event) = 0;

			/*!
			 * Do frame by frame updates.
			 */
			virtual void OnFrame() = 0;

			/*!
			 * \param Target The target to be rendered to
			 *
			 * Render the item onto a target.
			 */
			virtual void OnRender(sf::RenderTarget &Target) = 0;

			/*!
			 * Destroy the item.
			 */
			~Item();
	};
}

#endif

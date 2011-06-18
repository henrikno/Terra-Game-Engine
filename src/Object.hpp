#ifndef TERRA_OBJECT_HPP
#define TERRA_OBJECT_HPP

#include "Item.hpp"
#include "OgmoObject.hpp"

namespace terra{
	class Object : public Item{
		private:
			std::string Name;
		public:
			/*!
			 * \param ObjectData Information on the object that is being created
			 *
			 * Create a new object from received data.
			 */
			Object(OgmoObject ObjectData);

			/*!
			 * \return The type of the item
			 *
			 * Retrieve the type of the item.
			 */
			const ItemType GetItemType() const;

			/*!
			 * \return The name of the object
			 *
			 * Retrieve the name of the object.
			 */
			const std::string &GetName() const;

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
			 * Destroy the object.
			 */
			virtual ~Object();
	};
}

#endif

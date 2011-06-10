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
		private:
			sf::Vector2f Position;
			sf::Vector2<unsigned int> Size;
		public:
			/*!
			 * An enumeration of item types.
			 */
			enum ItemType{
				Tile,
				Object
			};

			/*!
			 * Create a new item.
			 */
			Item(sf::Vector2f InitialPosition = sf::Vector2f(0., 0.), sf::Vector2<unsigned int> InitialSize = sf::Vector2<unsigned int>(0, 0));

			/*!
			 * \return The type of the item
			 *
			 * Retrieve the type of the item.
			 */
			virtual const ItemType GetItemType() const = 0;

			/*!
			 * \return The position of the item
			 *
			 * Retrieve the position of the item.
			 */
			const sf::Vector2f &GetPosition() const;

			/*!
			 * \return The size of the item
			 *
			 * Retrieve the size of the item.
			 */
			const sf::Vector2<unsigned int> &GetSize() const;

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
			 * \param NewPosition The new position of the item
			 *
			 * Set the position of the item.
			 */
			void SetPosition(const sf::Vector2f &NewPosition);

			/*!
			 * \param NewPosition The new position of the item
			 *
			 * Set the position of the item.
			 */
			void SetSize(const sf::Vector2<unsigned int> &NewSize);

			/*!
			 * Destroy the item.
			 */
			~Item();
	};
}

#endif

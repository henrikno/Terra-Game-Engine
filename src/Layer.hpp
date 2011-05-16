#ifndef TERRA_LAYER_HPP
#define TERRA_LAYER_HPP

#include <list>
#include <memory>
#include "Item.hpp"

namespace terra{
	/*!
	 * \brief An Ogmo Layer
	 *
	 * An Ogmo Layer. It contains a layer of either grids, tiles, or objects.
	 */
	class Layer{
		private:
			std::list<std::shared_ptr<Item>> Items;
			Item::ItemType StoredItem;
		public:
			/*!
			 * \param StoredType The type of item that will be stored in the layer
			 *
			 * Create a new layer.
			 */
			Layer(Item::ItemType StoredType);

			/*!
			 * \param NewItem A shared pointer to the item to add
			 *
			 * Add an item to the layer.
			 */
			void AddItem(std::shared_ptr<Item> NewItem);

			/*!
			 * \return An iterator to the beginning of the item list
			 *
			 * Retrieve an iterator to the beginning of the item list.
			 */
			std::list<std::shared_ptr<Item>>::iterator Begin();

			/*!
			 * Remove all items from the item list.
			 */
			void Clear();

			/*!
			 * \return An iterator to the end of the item list
			 *
			 * Retrieve an iterator to the end of the item list.
			 */
			std::list<std::shared_ptr<Item>>::iterator End();

			/*!
			 * \return The type of item stored in the layer
			 *
			 * Retrieve the type of item stored in the layer.
			 */
			const Item::ItemType GetStoredType() const;

			/*!
			 * \param ItemIterator An iterator to the item to be removed
			 *
			 * Remove an item from the item list.
			 */
			void RemoveItem(std::list<std::shared_ptr<Item>>::iterator ItemIterator);

			/*!
			 * Destroy the layer.
			 */
			~Layer();
	};
}

#endif

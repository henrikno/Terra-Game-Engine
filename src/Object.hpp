#ifndef TERRA_OBJECT_HPP
#define TERRA_OBJECT_HPP

#include <SFML/Graphics.hpp>

namespace terra{
	/*!
	 * \brief Base object
	 *
	 * A base object from which all other game objects are derived.
	 */
	class Object{
		public:
			/*!
			 * Create an object.
			 */
			Object();

			/*!
			 * \param Event The event to be processed
			 *
			 * Process an event.
			 */
			virtual void OnEvent(sf::Event Event) = 0;

			/*!
			 * General updates to be run each frame.
			 */
			virtual void OnFrame() = 0;

			/*!
			 * \param Window The window to be rendered to
			 *
			 * Render the object onto the window.
			 */
			virtual void OnRender(sf::RenderTarget &Target) = 0;

			/*!
			 * Destroy the object.
			 */
			~Object();
	};
}

#endif

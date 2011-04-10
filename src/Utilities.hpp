#ifndef TERRA_UTILITIES_HPP
#define TERRA_UTILITIES_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>

namespace terra{
	/*!
	 * \param SoundName The filename of the sound
	 * \return A reference to the sound
	 *
	 * Loads a sound if it isn't already loaded, and returns it.
	 */
	sf::SoundBuffer &GetSound(std::string SoundName);

	/*!
	 * \param TextureName The filename of the texture
	 * \return A reference to the image
	 *
	 * Loads a texture if it isn't already loaded, and returns it.
	 */
	sf::Image &GetTexture(std::string TextureName);

	/*!
	 * \return True if the system is Big Endian, false if the system is Little Endian
	 *
	 * Uses pointer magic to check the endianness of the system at runtime.
	 */
	bool IsBigEndian();

	/*!
	 * \param A A convex polygon
	 * \param B Another convex polygon
	 * \return True if the two convex polygons are colliding, false otherwise
	 *
	 * Uses the separating axis theorem to quickly check if two convex polygons are colliding.
	 */
	bool IsColliding(sf::Shape A, sf::Shape B);

	/*!
	 * \param Bytes Pointer to the array of bytes to be endian-swapped
	 * \param NumBytes Length of the array
	 *
	 * Swaps the endianness of an array of bytes using pointer magic.
	 */
	void SwapEndianness(char *Bytes, unsigned long NumBytes);
}

#endif

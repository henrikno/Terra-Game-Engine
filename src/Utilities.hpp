#ifndef TERRA_UTILITIES_HPP
#define TERRA_UTILITIES_HPP

#include <list>
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace terra{
	/*!
	 * \param Shape The shape to check for concave points
	 * \return A list of point ids that are concave
	 *
	 * Detects all of the concave points in a shape.
	 */
	std::list<unsigned int> DetectConcavePoints(sf::Shape Shape);

	/*!
	 * \param SoundName The filename of the sound
	 * \return A reference to the sound
	 *
	 * Load a sound if it isn't already loaded, and return it.
	 */
	std::shared_ptr<sf::SoundBuffer> GetSound(std::string SoundName);

	/*!
	 * \param TextureName The filename of the texture
	 * \return A reference to the image
	 *
	 * Load a texture if it isn't already loaded, and return it.
	 */
	std::shared_ptr<sf::Image> GetTexture(std::string TextureName);

	/*!
	 * \return True if the system is Big Endian, false if the system is Little Endian
	 *
	 * Use pointer magic to check the endianness of the system at runtime.
	 */
	bool IsBigEndian();

	/*!
	 * \param A A convex polygon
	 * \param B Another convex polygon
	 * \return True if the two convex polygons are colliding, false otherwise
	 *
	 * Use the separating axis theorem to quickly check if two convex polygons are colliding.
	 */
	bool IsColliding(sf::Shape A, sf::Shape B);

	/*!
	 * Pause any music which is currently playing.
	 */
	void PauseMusic();

	/*!
	 * \param MusicName The name of the music to play
	 * \param Should the music be looped?
	 * \param Pause Should the music that is currently playing (if any) be paused instead of stopped?
	 *
	 * Open some music if it isn't already, and play it.
	 */
	void PlayMusic(std::string MusicName, bool Loop = false, bool Pause = false);

	/*!
	 * \param Filename The name of the file to read from
	 * \return The contents of the file
	 *
	 * Read in the contents of a file.
	 */
	std::vector<char> ReadFile(std::string Filename);

	/*!
	 * Stop any music which is currently playing.
	 */
	void StopMusic();

	/*!
	 * \param Bytes Pointer to the array of bytes to be endian-swapped
	 * \param NumBytes Length of the array
	 *
	 * Swap the endianness of an array of bytes using pointer magic.
	 */
	void SwapEndianness(char *Bytes, unsigned long NumBytes);
}

#endif

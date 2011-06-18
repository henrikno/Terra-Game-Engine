#ifndef TERRA_PERLINNOISE_HPP
#define TERRA_PERLINNOISE_HPP

#include <vector>

namespace terra{
	/*!
	 * \brief A Perlin Noise generator
	 *
	 * A Perlin Noise generator. Useful for level generation, texture generation, and other things.
	 */
	class PerlinNoise{
		private:
			unsigned int Seed;
			double Interpolate(double A, double B, double X);
			double GetSmooth(int X);
			double GetSmooth(int X, int Y);
			double Rand(int X);
			double Rand(int X, int Y);
		public:
			/*!
			 * \param NewSeed The seed of the noise generator
			 *
			 * Create a new Perlin Noise generator and initialize it.
			 */
			PerlinNoise(unsigned int NewSeed);

			/*!
			 * \param X The coordinate of the noise value
			 * \param Octaves The number of octaves of Perlin Noise
			 * \param Persistence The persistence of the octaves
			 * \param ScalingX The scaling value for the x coordinate between octaves
			 *
			 * Retrieve a noise value on the one-dimensional plane.
			 */
			double Noise1D(double X, unsigned int Octaves = 1, double Persistence = 0.5, double ScalingX = 2.);

			/*!
			 * \param X The x coordinate of the noise value
			 * \param Y The y coordinate of the noise value
			 * \param Octaves The number of octaves of Perlin Noise
			 * \param Persistence The persistence of the octaves
			 * \param ScalingX The scaling value for the x coordinate between octaves
			 * \param ScalingY The scaling value for the y coordinate between octaves
			 *
			 * Retrieve a noise value on the two-dimensional plane.
			 */
			double Noise2D(double X, double Y, unsigned int Octaves = 1, double Persistence = 0.5, double ScalingX = 2., double ScalingY = 2.);

			/*!
			 * \param NewSeed The new seed of the noise generator
			 *
			 * Reset the seed of the noise generator.
			 */
			void Reseed(unsigned int NewSeed);

			/*!
			 * Destroy the Perlin Noise generator.
			 */
			~PerlinNoise();
	};
}

#endif

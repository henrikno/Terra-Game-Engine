#ifndef TERRA_ENGINE_HPP
#define TERRA_ENGINE_HPP

#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include "NBT.hpp"
#include "Object.hpp"

namespace terra{
	class Engine{
		private:
			sf::RenderWindow GameWindow;
			bool NewLevel;
			std::string NewLevelName;
			std::map<std::string, std::shared_ptr<terra::Object> (*)(nbt::Compound*)> ObjectCallbacks;
			std::multimap<int, std::shared_ptr<terra::Object>> ObjectList;
			bool Ready;

			Engine();
			Engine(const terra::Engine &Copy);
			terra::Engine &operator=(const terra::Engine &Copy);
			void PrepareLevel();
		public:
			/*!
			 * \return Reference to the singleton instance
			 *
			 * Retrieve a reference to the singleton instance.
			 */
			static terra::Engine &Get();

			std::multimap<int, std::shared_ptr<terra::Object>> &GetObjects();

			/*!
			 * \return The major version of OpenGL
			 *
			 * Retrieve the major version of OpenGL.
			 */
			const int GetOpenGLMajorVersion() const;

			/*!
			 * \return The minor version of OpenGL
			 *
			 * Retrieve the minor version of OpenGL.
			 */
			const int GetOpenGLMinorVersion() const;

			/*!
			 * \return Reference to the game window
			 *
			 * Retrieve a reference to the game window.
			 */
			sf::RenderWindow &GetWindow();

			/*!
			 * \param argc The argc variable in main()
			 * \param argv The argv variable in main()
			 *
			 * Process any arguments passed on the command line. MUST BE CALLED BEFORE Run()!
			 */
			void Initialize(int argc, char *argv[]);

			/*!
			 * \param LevelName The filename of the level to load
			 *
			 * Loads a level from a specified path, where the root directory is the executable's directory.
			 */
			void LoadLevel(std::string LevelName);

			/*!
			 * \return The return code for main()
			 *
			 * The game loop. Call at the end of main() and return its result.
			 */
			int Main();

			/*!
			 * \param Name The name of the object, which is used in level files
			 * \param CallbackConstructor A callback function which takes an NBT Compound containing information on the object, and returns a shared pointer to an instance of the object
			 *
			 * Registers an object with the engine for use in level loading, consisting of a string to identify the object, and a callback to create an instance of the object.
			 */
			void Register(std::string Name, std::shared_ptr<terra::Object> (*CallbackConstructor)(nbt::Compound*));

			/*!
			 * Destroy the engine.
			 */
			~Engine();
	};
}

#endif

#ifndef TERRA_ENGINE_HPP
#define TERRA_ENGINE_HPP

#include <list>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include "Layer.hpp"
#include "Object.hpp"
#include "OgmoObject.hpp"
#include "RapidXML.hpp"

namespace terra{
	/*!
	 * \brief The game engine
	 *
	 * The game engine. There can only be one instance of it. It does pretty much everything to keep the game running.
	 */
	class Engine{
		private:
			std::map<std::string, std::shared_ptr<Object> (*)(const OgmoObject &)> Callbacks;
			std::list<std::pair<unsigned int, std::string>> ConsoleLog;
			bool ConsoleOpen;
			bool Initialized;
			std::list<std::shared_ptr<Layer>> Layers;
			std::map<std::string, std::shared_ptr<Layer>> NamedLayers;
			bool NewLevel;
			std::string NextLevelName;
			std::map<std::string, OgmoObject> OgmoObjects;
			sf::RenderWindow Window;

			void ParseLevel();
			void ParseObjectFolder(rapidxml::xml_node<> *Folder);
			void ParseProject();

			Engine();
			Engine(const Engine &Copy);
			Engine &operator=(const Engine &Copy);
		public:
			/*!
			 * \param ErrorMessage A message describing the error
			 *
			 * Sends an error to the console.
			 */
			void Error(const std::string &ErrorMessage);

			/*!
			 * \return A reference to the Engine's singleton instance
			 *
			 * Retrieve a reference to the Engine's singleton instance.
			 */
			static Engine &Get();

			/*!
			 * \param Name The name of the layer to retrieve
			 * \return A shared pointer to the layer with the given name
			 *
			 * Retrieve a shared pointer to the layer with the given name.
			 */
			std::shared_ptr<Layer> GetLayer(std::string Name);

			/*!
			 * \return A reference to the game window
			 *
			 * Retrieve a reference to the game window.
			 */
			sf::RenderWindow &GetWindow();

			/*!
			 * \param argc The argc variable from main()
			 * \param argv The argv variable from main()
			 *
			 * Processes the variables passed to the engine, and prepares the game.
			 */
			void Initialize(const int argc, char *argv[]);

			/*!
			 * \param Filename The filename of the level to be loaded
			 *
			 * Loads a new level, which replaces the current one in memory. The level loading will not run until the beginning of the next frame.
			 */
			void LoadLevel(std::string Filename);

			/*!
			 * \return 0 if the game ended successfully, anything else means an error
			 *
			 * Runs the game. You MUST call Initialize() first!
			 */
			int Main();

			/*!
			 * \param TheMessage A message
			 *
			 * Sends a message to the console.
			 */
			void Message(const std::string &TheMessage);

			/*!
			 * \param Name The name for the object used in the Ogmo Editor
			 * \param Callback A callback function which creates a new shared pointer to the object
			 *
			 * Register a new object in the game engine so it can be parsed out of the Ogmo Levels and into the game.
			 */
			void RegisterObject(std::string Name, std::shared_ptr<Object> (*Callback)(const OgmoObject &));

			/*!
			 * \param WarningMessage A message describing the warning
			 *
			 * Sends a warning to the console.
			 */
			void Warning(const std::string &WarningMessage);

			/*!
			 * Destroy the Engine.
			 */
			~Engine();
	};
};

#endif

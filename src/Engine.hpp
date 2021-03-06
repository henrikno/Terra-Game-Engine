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
#include "OgmoTileLayer.hpp"
#include "OgmoTileset.hpp"
#include "RapidXML.hpp"

namespace terra{
	/*!
	 * \brief The game engine
	 *
	 * The game engine. There can only be one instance of it. It does pretty much everything to keep the game running.
	 */
	class Engine{
		private:
			std::map<std::string, std::shared_ptr<Item> (*)(const OgmoObject &)> Callbacks;
			std::list<std::pair<unsigned int, std::string>> ConsoleLog;
			bool ConsoleOpen;
			bool Initialized;
			std::list<std::shared_ptr<Layer>> Layers;
			std::map<std::string, std::shared_ptr<Layer>> NamedLayers;
			bool NewLevel;
			std::string NextLevelName;
			sf::RenderWindow Window;

			// Ogmo Level Stuff
			std::map<std::string, std::string> DefaultLevelValues;
			std::map<std::string, std::string> LevelValues;
			std::map<std::string, std::string> ValueTypes;

			// Ogmo Tileset Stuff
			std::map<std::string, OgmoTileset> OgmoTilesets;
			std::map<std::string, OgmoTileLayer> OgmoTileLayers;

			// Ogmo Object Stuff
			std::map<std::string, OgmoObject> OgmoObjects;

			// Parsers
			void ParseBoot(unsigned int &Width, unsigned int &Height, unsigned int &Framerate, std::string &Title, std::string &InitialLevel);
			void ParseCommandLine(const int argc, char *argv[], unsigned int &Width, unsigned int &Height);
			void ParseLayers(rapidxml::xml_node<> *Root);
			void ParseLevel();
			void ParseLevelObjectLayer(rapidxml::xml_node<> *ObjectLayer);
			void ParseLevelTileLayer(rapidxml::xml_node<> *TileLayer);
			void ParseObjects(rapidxml::xml_node<> *Root);
			void ParseObjectFolder(rapidxml::xml_node<> *Folder);
			void ParseObjectLayer(rapidxml::xml_node<> *ObjectLayer);
			void ParseProject();
			void ParseTileLayer(rapidxml::xml_node<> *TileLayer);
			void ParseTilesets(rapidxml::xml_node<> *Root);

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
			 * \param Name The name of the value to retrieve
			 * \return The value of the value
			 *
			 * Retrieve a value from the level.
			 */
			std::string GetLevelValue(std::string Name);

			/*!
			 * \param Name The name of the value whose type is to be retrieved
			 * \return The type of the value
			 *
			 * Retrieve the type of a value from the level.
			 */
			std::string GetLevelValueType(std::string Name);

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
			 * \return True if the console is open, false otherwise
			 *
			 * Determines if the console is open or not. Useful for stopping animations in the OnRender event if the console is open.
			 */
			const bool IsConsoleOpen() const;

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
			void RegisterObject(std::string Name, std::shared_ptr<Item> (*Callback)(const OgmoObject &));

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

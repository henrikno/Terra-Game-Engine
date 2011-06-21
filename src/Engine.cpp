#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "Engine.hpp"
#include "Item.hpp"
#include "OgmoTile.hpp"
#include "Tile.hpp"
#include "Utilities.hpp"

terra::Engine::Engine(){
	// Just setting up some variables
	ConsoleOpen = false;
	Initialized = false;
}

void terra::Engine::Error(const std::string &ErrorMessage){
	// Keep an error log, mark errors with id 2 for color-coding
	ConsoleLog.push_back(std::pair<unsigned int, std::string>(2, ErrorMessage));
}

terra::Engine &terra::Engine::Get(){
	// Keeps a singleton (no use of new, C++0x requires proper initialization in multithreaded environments, iirc)
	static terra::Engine Singleton;
	return Singleton;
}

std::shared_ptr<terra::Layer> terra::Engine::GetLayer(std::string Name){
	// Give a null pointer if the layer doesn't exist
	if (NamedLayers.find(Name) == NamedLayers.end())
		return std::shared_ptr<terra::Layer>(nullptr);

	// Give the correct layer otherwise
	return NamedLayers[Name];
}

std::string terra::Engine::GetLevelValue(std::string Name){
	if (LevelValues.find(Name) == LevelValues.end()){
		Warning(std::string("Level Value of name \"") + Name + "\" does not exist\n");
		return "";
	}
	return LevelValues[Name];
}

std::string terra::Engine::GetLevelValueType(std::string Name){
	if (ValueTypes.find(Name) == ValueTypes.end()){
		Warning(std::string("Level Value of name \"") + Name + "\" does not exist\n");
		return "";
	}
	return ValueTypes[Name];
}

sf::RenderWindow &terra::Engine::GetWindow(){
	return Window;
}

void terra::Engine::Initialize(const int argc, char *argv[]){
	// Some initial values
	unsigned int Width = 640, Height = 480, Framerate = 60;
	std::string Title = "Terra Game Engine", InitialLevel = "res/levels/Title.oel";

	// Parse the boot file
	ParseBoot(Width, Height, Framerate, Title, InitialLevel);

	// Parse the command line
	ParseCommandLine(argc, argv, Width, Height);

	// Begin the actual initialization
	NewLevel = true;
	NextLevelName = InitialLevel;

	// Parse the game's Ogmo Editor project to extract any needed info
	ParseProject();

	// Finish initialization
	Window.Create(sf::VideoMode(Width, Height, 32), Title);
	Initialized = true;
}

const bool terra::Engine::IsConsoleOpen() const{
	return ConsoleOpen;
}

void terra::Engine::LoadLevel(std::string Filename){
	// Don't actually load the level, but prepare for loading at the beginning of the next frame
	NewLevel = true;
	NextLevelName = Filename;
}

int terra::Engine::Main(){
	// Check if the developer forgot to initialize the engine
	if (!Initialized){
		Error("Game attempted to run before initialization!\n");
		return -1;
	}

	// Prepare the console
	sf::Font ConsoleFont;
	if (!ConsoleFont.LoadFromFile("res/fonts/DejaVuSansMono.ttf")){
		Warning("Unable to locate the console font. Going with default Arial.");
		ConsoleFont = sf::Font::GetDefaultFont();
	}

	// Main Loop
	while (Window.IsOpened()){
		// Main gameplay
		if (!ConsoleOpen){
			// Level loading
			if (NewLevel)
				ParseLevel();

			// Event handling
			sf::Event Event;
			while (Window.PollEvent(Event)){
				if (Event.Type == sf::Event::Closed)
					Window.Close();
				else if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
					ConsoleOpen = true;
				else
					for (auto i = Layers.begin(); i != Layers.end(); ++i)
						for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
							(*j)->OnEvent(Event);
			}

			// Game Logic
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnFrame();

			// Game Rendering
			Window.Clear();
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnRender(Window);
			Window.Display();
		}
		// Console Control
		else{
			// Event handling
			sf::Event Event;
			while (Window.PollEvent(Event)){
				if (Event.Type == sf::Event::Closed)
					Window.Close();
				else if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
					ConsoleOpen = false;
			}

			// Render the game's objects as if they were floating in the background
			Window.Clear();
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnRender(Window);

			// Darken the screen
			sf::View Temp = Window.GetView();
			sf::View Replacement(sf::FloatRect(0, 0, Window.GetWidth(), Window.GetHeight()));
			Window.SetView(Replacement);
			Window.Draw(sf::Shape::Rectangle(0., 0., Window.GetWidth(), Window.GetHeight(), sf::Color(0, 0, 0, 170)));

			// Draw the console output
			unsigned int LineCounter = 0;
			unsigned int LineMax = Window.GetHeight()/20;
			for (auto i = ConsoleLog.rbegin(); i != ConsoleLog.rend(); ++i){
				// End the console rendering if too many lines
				if (LineCounter >= LineMax)
					break;

				// Prepare the text
				unsigned int LineY = Window.GetHeight()-(LineCounter+1)*20;
				sf::Text Line(i->second, ConsoleFont, 16);

				// Color code warnings and errors
				if (i->first == 1)
					Line.SetColor(sf::Color::Yellow);
				else if (i->first == 2)
					Line.SetColor(sf::Color::Red);

				// Set the positions
				Line.SetX(2);
				Line.SetY(LineY);

				// And Render!
				Window.Draw(Line);
				++LineCounter;
			}
			Window.Display();
			Window.SetView(Temp);
		}
	}
	return 0;
}

void terra::Engine::Message(const std::string &TheMessage){
	ConsoleLog.push_back(std::pair<unsigned int, std::string>(0, TheMessage));
}

void terra::Engine::ParseBoot(unsigned int &Width, unsigned int &Height, unsigned int &Framerate, std::string &Title, std::string &InitialLevel){
	// Open up the Boot file and parse it with RapidXML
	std::vector<char> Contents = terra::ReadFile("res/cfg/Boot.cfg");
	rapidxml::xml_document<> Boot;
	Boot.parse<0>(&Contents[0]);

	// Check for the root
	rapidxml::xml_node<> *Root = Boot.first_node("config");
	if (Root == nullptr || Root->type() != rapidxml::node_element){
		Error("Boot file \"res/cfg/Boot.cfg\" has no root node\n");
		return;
	}

	// Parse the window properties
	rapidxml::xml_node<> *WindowAttributes = Root->first_node("window");
	if (WindowAttributes != nullptr && WindowAttributes->type() == rapidxml::node_element){
		// Read in the properties
		rapidxml::xml_node<> *WindowWidth = WindowAttributes->first_node("width");
		rapidxml::xml_node<> *WindowHeight = WindowAttributes->first_node("height");
		rapidxml::xml_node<> *WindowFramerate = WindowAttributes->first_node("framerate");
		rapidxml::xml_node<> *WindowTitle = WindowAttributes->first_node("title");

		// Translate strings into numbers
		if (WindowWidth != nullptr && WindowWidth->type() == rapidxml::node_element && WindowWidth->first_node()->type() == rapidxml::node_data){
			int Temp = atoi(WindowWidth->first_node()->value());
			Width = Temp > 0 ? Temp : Width;
		}
		if (WindowHeight != nullptr && WindowHeight->type() == rapidxml::node_element && WindowHeight->first_node()->type() == rapidxml::node_data){
			int Temp = atoi(WindowHeight->first_node()->value());
			Height = Temp > 0 ? Temp : Height;
		}
		if (WindowFramerate != nullptr && WindowFramerate->type() == rapidxml::node_element && WindowFramerate->first_node()->type() == rapidxml::node_data){
			int Temp = atoi(WindowFramerate->first_node()->value());
			Framerate = Temp > 0 ? Temp : Framerate;
		}
		if (WindowTitle != nullptr && WindowTitle->type() == rapidxml::node_element && WindowTitle->first_node()->type() == rapidxml::node_data)
			Title = WindowTitle->first_node()->value();
	}

	// We also need the initial level's filename
	if (Root->first_node("level") != nullptr && Root->first_node("level")->type() == rapidxml::node_element && Root->first_node("level")->first_node() != nullptr && Root->first_node("level")->first_node()->type() == rapidxml::node_data)
		InitialLevel = Root->first_node("level")->first_node()->value();
}

void terra::Engine::ParseCommandLine(const int argc, char *argv[], unsigned int &Width, unsigned int &Height){
	// Translate the command line arguments into a list
	std::list<std::string> ArgumentList;
	for (int i = 1; i < argc; ++i)
		ArgumentList.push_back(argv[i]);

	// Translate the commands into values
	for (auto i = ArgumentList.begin(); i != ArgumentList.end(); ++i){
		if (*i == "-width"){
			int Temp = atoi((++i)->c_str());
			Width = Temp > 0 ? Temp : Width;
		}
		else if (*i == "-height"){
			int Temp = atoi((++i)->c_str());
			Height = Temp > 0 ? Temp : Height;
		}
	}
}

void terra::Engine::ParseLayers(rapidxml::xml_node<> *Root){
	for (auto Layers = Root->first_node("layers"); Layers != nullptr; Layers = Layers->next_sibling("layers")){
		// Skip invalid layers
		if (Layers->type() != rapidxml::node_element)
			continue;

		// Parse Tile Layers
		for (auto LayerIterator = Layers->first_node("tiles"); LayerIterator != nullptr; LayerIterator = LayerIterator->next_sibling("tiles")){
			if (LayerIterator->type() != rapidxml::node_element)
				continue;
			ParseTileLayer(LayerIterator);
		}

		// Parse Object Layers
		for (auto LayerIterator = Layers->first_node("objects"); LayerIterator != nullptr; LayerIterator = LayerIterator->next_sibling("objects")){
			if (LayerIterator->type() != rapidxml::node_element)
				continue;
			ParseObjectLayer(LayerIterator);
		}
	}
}

void terra::Engine::ParseLevel(){
	// Clear out the old level and tell the engine that the level has loaded (or at least tried to)
	for (auto i = Layers.begin(); i != Layers.end(); ++i)
		(*i)->Clear();
	LevelValues = DefaultLevelValues;
	NewLevel = false;

	// Read in the level file and parse it with RapidXML
	std::vector<char> Contents = terra::ReadFile(NextLevelName);
	rapidxml::xml_document<> Level;
	Level.parse<0>(&Contents[0]);

	// Check for the root node
	rapidxml::xml_node<> *LevelRoot = Level.first_node("level");
	if (LevelRoot == nullptr || LevelRoot->type() != rapidxml::node_element){
		Error(std::string("Level file \"") + NextLevelName + "\" has no root node\n");
		return;
	}

	// Parse the level's values
	for (auto i = LevelRoot->first_attribute(); i != nullptr; i = i->next_attribute()){
		// Get the value properties
		std::string Name = i->name();
		std::string Value = i->value();

		// Validate that the value exists
		if (LevelValues.find(Name) == LevelValues.end()){
			Warning(std::string("Level value of name \"") + Name + "\" does not exist\n");
			continue;
		}

		// Set the value
		LevelValues[Name] = Value;
	}

	// Begin parsing the layers that were registered in the project file
	for (auto i = NamedLayers.begin(); i != NamedLayers.end(); ++i){
		// Check if the layer exists
		rapidxml::xml_node<> *LayerIterator = LevelRoot->first_node(i->first.c_str());
		if (LayerIterator == nullptr || LayerIterator->type() != rapidxml::node_element)
			continue;

		// Parse the Tile Layers
		if (i->second->GetStoredType() == terra::Item::Tile)
			ParseLevelTileLayer(LayerIterator);

		// Parse the Object Layers
		if (i->second->GetStoredType() == terra::Item::Object)
			ParseLevelObjectLayer(LayerIterator);
	}
}

void terra::Engine::ParseLevelObjectLayer(rapidxml::xml_node<> *ObjectLayer){
	// Process each object in the layer
	for (auto Object = ObjectLayer->first_node(); Object != nullptr; Object = Object->next_sibling()){
		// Verify that the object is valid
		if (Object->type() != rapidxml::node_element || Object->first_attribute("x") == nullptr || Object->first_attribute("y"))
			continue;

		// Verify that the object exists
		if (OgmoObjects.find(Object->name()) == OgmoObjects.end()){
			Warning(std::string("Object of name \"") + Object->name() + "\" does not exist\n");
			continue;
		}

		// Parse the properties of the object
		terra::OgmoObject NewObject;
		NewObject.Name = Object->name();
		NewObject.Position = sf::Vector2f(atof(Object->first_attribute("x")->value()), atof(Object->first_attribute("y")->value()));
		if (NewObject.ResizableX && Object->first_attribute("width") != nullptr)
			NewObject.Size.x = atoi(Object->first_attribute("width")->value());
		if (NewObject.ResizableY && Object->first_attribute("height") != nullptr)
			NewObject.Size.y = atoi(Object->first_attribute("height")->value());

		// Parse the values of the object
		for (auto i = Object->first_attribute(); i != nullptr; i = i->next_attribute()){
			// Skip non-values
			if (i->name() == "x" || i->name() == "y" || i->name() == "width" || i->name() == "height")
				continue;

			// Get the properties
			std::string Name = i->name();
			std::string Value = i->value();

			// Verify that the value exists
			if (NewObject.Values.find(Name) == NewObject.Values.end()){
				Warning(std::string("Value of name \"") + Name + "\" does not exist\n");
				continue;
			}

			// Update the value
			NewObject.Values[Name] = Value;
		}

		// Validate that the object is registered
		if (Callbacks.find(NewObject.Name) == Callbacks.end()){
			Warning(std::string("Object of name\"") + NewObject.Name + "\" is not registered\n");
			continue;
		}

		// Insert the object
		NamedLayers[ObjectLayer->name()]->AddItem(Callbacks[NewObject.Name](NewObject));
	}
}

void terra::Engine::ParseLevelTileLayer(rapidxml::xml_node<> *TileLayer){
	// Give up if the tileset is needed but not given
	if (TileLayer->first_attribute("set") == nullptr && !OgmoTileLayers[TileLayer->name()].MultipleTilesets)
		return;

	// Prepare the tileset name (if needed)
	std::string Set;
	if (!OgmoTileLayers[TileLayer->name()].MultipleTilesets){
		Set = TileLayer->first_attribute("set")->value();
		if (OgmoTilesets.find(Set) == OgmoTilesets.end() || !GetTexture(Set))
			return;
	}

	// Read in the tile size (if needed)
	unsigned int TileWidth;
	unsigned int TileHeight;
	if (OgmoTileLayers[TileLayer->name()].ExportTileSize){
		if (TileLayer->first_attribute("tileWidth") == nullptr || TileLayer->first_attribute("tileHeight") == nullptr)
			return;
		TileWidth = atoi(TileLayer->first_attribute("tileWidth")->value());
		TileHeight = atoi(TileLayer->first_attribute("tileHeight")->value());
	}
	else if (!OgmoTileLayers[TileLayer->name()].MultipleTilesets){
		TileWidth = OgmoTilesets[Set].TileWidth;
		TileHeight = OgmoTilesets[Set].TileHeight;
	}

	// And now we parse the tiles
	for (auto TileIterator = TileLayer->first_node("tile"); TileIterator != nullptr; TileIterator = TileIterator->next_sibling("tile")){
		// First check if the tile is valid
		if (TileIterator->type() != rapidxml::node_element || TileIterator->first_attribute("x") == nullptr || TileIterator->first_attribute("y") == nullptr)
			continue;

		// Now read in the position
		sf::Vector2f Position = sf::Vector2f(atof(TileIterator->first_attribute("x")->value()), atof(TileIterator->first_attribute("y")->value()));
		sf::Vector2<unsigned int> TilePosition;

		// Check if the needed information about tilesets is given or not (if needed)
		if (OgmoTileLayers[TileLayer->name()].MultipleTilesets){
			if (TileIterator->first_attribute("set") == nullptr)
				continue;
			Set = TileIterator->first_attribute("set")->value();
			if (OgmoTilesets.find(Set) == OgmoTilesets.end() || !GetTexture(Set))
				continue;
		}
		if (!OgmoTileLayers[TileLayer->name()].ExportTileSize && OgmoTileLayers[TileLayer->name()].MultipleTilesets){
			TileWidth = OgmoTilesets[Set].TileWidth;
			TileHeight = OgmoTilesets[Set].TileHeight;
		}

		// Parse the position of the tile in the tileset
		if (OgmoTileLayers[TileLayer->name()].ExportTileIDs){
			// Check if the tile id is given
			if (TileIterator->first_attribute("id") == nullptr)
				continue;

			// Parse some information such as the id number and the size of each tile id
			unsigned int ID = atoi(TileIterator->first_attribute("id")->value());
			unsigned int IDWidth = (GetTexture(Set)->GetWidth()-GetTexture(Set)->GetWidth()%TileWidth)/TileWidth;
			unsigned int IDHeight = (GetTexture(Set)->GetHeight()-GetTexture(Set)->GetHeight()%TileHeight)/TileHeight;

			// Ensure that the id isn't too large
			if (ID >= IDWidth*IDHeight)
				continue;

			// Finally, calculate the tile position
			TilePosition.x = ID%IDWidth*TileWidth;
			TilePosition.y = (ID-ID%IDWidth)/IDWidth*TileHeight;
		}
		else{
			// Check if the tile position is given
			if (TileIterator->first_attribute("tx") == nullptr || TileIterator->first_attribute("ty") == nullptr)
				continue;

			// We don't have to do math here like we did with ided tiles. Yay
			TilePosition.x = atoi(TileIterator->first_attribute("tx")->value());
			TilePosition.y = atoi(TileIterator->first_attribute("ty")->value());
		}

		// Prepare the tile for insertion
		terra::OgmoTile NextTile;
		NextTile.Tileset = Set;
		NextTile.TilePosition = TilePosition;
		NextTile.TileSize = sf::Vector2<unsigned int>(TileWidth, TileHeight);
		NextTile.Position = Position;

		// Insert the tile into the layer
		NamedLayers[TileLayer->name()]->AddItem(std::shared_ptr<terra::Item>(new terra::Tile(NextTile)));
	}
}

void terra::Engine::ParseObjects(rapidxml::xml_node<> *Root){
	// Just a wrapper to parse all object folders
	for (auto Folder = Root->first_node("objects"); Folder != nullptr; Folder = Folder->next_sibling("objects"))
		ParseObjectFolder(Folder);
}

void terra::Engine::ParseObjectFolder(rapidxml::xml_node<> *Folder){
	// Parse all subfolders
	for (auto NextFolder = Folder->first_node("folder"); NextFolder != nullptr; NextFolder = NextFolder->next_sibling("folder"))
		ParseObjectFolder(NextFolder);

	// Then parse all objects
	for (auto Object = Folder->first_node("object"); Object != nullptr; Object = Object->next_sibling("object")){
		// Make sure the object is safe
		if (Object->type() != rapidxml::node_element || Object->first_attribute("name") == nullptr || Object->first_attribute("width") == nullptr || Object->first_attribute("height") == nullptr)
			continue;

		// Prepare the object for use in future object creations
		terra::OgmoObject NextObject;
		NextObject.Name = Object->first_attribute("name")->value();
		NextObject.Size = sf::Vector2<unsigned int>(atoi(Object->first_attribute("width")->value()), atoi(Object->first_attribute("height")->value()));
		bool ResizableX = false;
		bool ResizableY = false;
		if (Object->first_attribute("resizableX") != nullptr && Object->first_attribute("resizableX")->value() == "true")
			ResizableX = true;
		if (Object->first_attribute("resizableY") != nullptr && Object->first_attribute("resizableY")->value() == "true")
			ResizableY = true;
		NextObject.ResizableX = ResizableX;
		NextObject.ResizableY = ResizableY;

		// Parse the default values
		for (auto ValueContainer = Object->first_node("values"); ValueContainer != nullptr; ValueContainer = ValueContainer->next_sibling("values")){
			// Verify that the value container is valid
			if (ValueContainer->type() != rapidxml::node_element)
				continue;

			// Parse all values
			for (auto Value = ValueContainer->first_node(); Value != nullptr; Value = Value->next_sibling()){
				// Validate the value
				if (Value->type() != rapidxml::node_element || (Value->name() != "boolean" && Value->name() != "integer" && Value->name() != "number" && Value->name() != "string" && Value->name() != "text") || Value->first_attribute("name") == nullptr || Value->first_attribute("default") == nullptr)
					continue;

				// Get the value's properties
				std::string Name = Value->first_attribute("name")->value();
				std::string Default = Value->first_attribute("default")->value();

				// Validate that it isn't a duplicate
				if (NextObject.Values.find(Name) != NextObject.Values.end()){
					Warning(std::string("Value of name \"") + Name + "\" already exists\n");
					continue;
				}

				// Add the value to the default values
				NextObject.Values[Name] = Default;
				NextObject.ValueTypes[Name] = Value->name();
			}
		}

		// Make sure the object isn't duplicated
		if (OgmoObjects.find(NextObject.Name) != OgmoObjects.end()){
			Warning(std::string("Object of name \"") + NextObject.Name + "\" already exists\n");
			continue;
		}

		// Make sure the object is registered
		if (Callbacks.find(NextObject.Name) == Callbacks.end()){
			Warning(std::string("Object of name\"") + NextObject.Name + "\" hasn't been registered\n");
			continue;
		}

		// And insert
		OgmoObjects[NextObject.Name] = NextObject;
	}
}

void terra::Engine::ParseObjectLayer(rapidxml::xml_node<> *ObjectLayer){
	// Verify that the layer is valid
	if (ObjectLayer->first_attribute("name") == nullptr)
		return;

	// Extract the name
	std::string Name = ObjectLayer->first_attribute("name")->value();

	// Verify that the layer isn't a duplicate
	if (NamedLayers.find(Name) != NamedLayers.end()){
		Warning(std::string("Layer of name \"") + Name + "\" already exists\n");
		return;
	}

	// Store the new layer
	std::shared_ptr<terra::Layer> NewLayer(new terra::Layer(terra::Item::Object));
	NamedLayers[Name] = NewLayer;
	Layers.push_back(NewLayer);
}

void terra::Engine::ParseProject(){
	// Read the project file in and parse it with RapidXML
	std::vector<char> Contents = terra::ReadFile("res/cfg/Levels.oep");
	rapidxml::xml_document<> Project;
	Project.parse<0>(&Contents[0]);

	// Get the project root
	rapidxml::xml_node<> *ProjectRoot = Project.first_node("project");

	// Make sure the project is valid
	if (ProjectRoot == nullptr || ProjectRoot->type() != rapidxml::node_element){
		Error("Project file \"res/cfg/Levels.oep\" has no root node\n");
		return;
	}

	// Parse default values
	for (auto ValueContainer = ProjectRoot->first_node("values"); ValueContainer != nullptr; ValueContainer = ValueContainer->next_sibling("values")){
		// Verify that the value container is valid
		if (ValueContainer->type() != rapidxml::node_element)
			continue;

		// Parse all values
		for (auto Value = ValueContainer->first_node(); Value != nullptr; Value = Value->next_sibling()){
			// Validate the value
			if (Value->type() != rapidxml::node_element || (Value->name() != "boolean" && Value->name() != "integer" && Value->name() != "number" && Value->name() != "string" && Value->name() != "text") || Value->first_attribute("name") == nullptr || Value->first_attribute("default") == nullptr)
				continue;

			// Get the value's properties
			std::string Name = Value->first_attribute("name")->value();
			std::string Default = Value->first_attribute("default")->value();

			// Validate that it isn't a duplicate
			if (DefaultLevelValues.find(Name) != DefaultLevelValues.end()){
				Warning(std::string("Level value of name \"") + Name + "\" already exists\n");
				continue;
			}

			// Add the value to the default values
			DefaultLevelValues[Name] = Default;
			ValueTypes[Name] = Value->name();
		}
	}

	// Parse everything
	ParseTilesets(ProjectRoot);
	ParseObjects(ProjectRoot);
	ParseLayers(ProjectRoot);
}

void terra::Engine::ParseTileLayer(rapidxml::xml_node<> *TileLayer){
	// Validate the layer
	if (TileLayer->first_attribute("name") == nullptr)
		return;

	// Some Initial Values
	std::string Name = TileLayer->first_attribute("name")->value();
	bool MultipleTilesets = false;
	bool ExportTileSize = false;
	bool ExportTileIDs = false;

	// Get the optional values
	if (TileLayer->first_attribute("multipleTilesets") != nullptr && TileLayer->first_attribute("multipleTilesets")->value() == "true")
		MultipleTilesets = true;
	if (TileLayer->first_attribute("exportTileSize") != nullptr && TileLayer->first_attribute("exportTileSize")->value() == "true")
		ExportTileSize = true;
	if (TileLayer->first_attribute("exportTileIDs") != nullptr && TileLayer->first_attribute("exportTileIDs")->value() == "true")
		ExportTileIDs = true;

	// Ensure that the layer isn't a duplicate
	if (NamedLayers.find(Name) != NamedLayers.end()){
		Warning(std::string("Layer of name \"") + Name + "\" already exists\n");
		return;
	}

	// Prepare the new layer for storage
	terra::OgmoTileLayer NewTileLayer;
	NewTileLayer.MultipleTilesets = MultipleTilesets;
	NewTileLayer.ExportTileSize = ExportTileSize;
	NewTileLayer.ExportTileIDs = ExportTileIDs;

	// Store the new layer
	OgmoTileLayers[Name] = NewTileLayer;
	std::shared_ptr<terra::Layer> NewLayer(new terra::Layer(terra::Item::Tile));
	NamedLayers[Name] = NewLayer;
	Layers.push_back(NewLayer);
}

void terra::Engine::ParseTilesets(rapidxml::xml_node<> *Root){
	// Parse the working directory
	std::string WorkingDirectory = "res/cfg/";
	for (auto Settings = Root->first_node("settings"); Settings != nullptr; Settings = Settings->next_sibling("settings")){
		// Validate the settings node
		if (Settings->type() != rapidxml::node_element)
			continue;

		// Hunt for the working directory
		bool Found = false;
		for (auto WorkingDirectoryNode = Settings->first_node("workingDirectory"); WorkingDirectoryNode != nullptr; WorkingDirectoryNode = WorkingDirectoryNode->next_sibling("workingDirectory")){
			// Validate the Working Directory Node
			if (WorkingDirectoryNode->type() != rapidxml::node_element || WorkingDirectoryNode->first_node() == nullptr || WorkingDirectoryNode->first_node()->type() != rapidxml::node_data)
				continue;

			// Update the working directory then evacuate
			WorkingDirectory += WorkingDirectoryNode->first_node()->value();
			Found = true;
			break;
		}
		if (Found)
			break;
	}

	// Parse the tilesets
	for (auto i = Root->first_node("tilesets"); i != nullptr; i = i->next_sibling("tilesets")){
		// Validate the tileset container node
		if (i->type() != rapidxml::node_element)
			continue;

		// Parse each tileset in the list
		for (auto j = i->first_node("tileset"); j != nullptr; j = j->next_sibling("tileset")){
			// Validate the tileset
			if (j->type() != rapidxml::node_element || j->first_attribute("name") == nullptr || j->first_attribute("image") == nullptr || j->first_attribute("tileWidth") == nullptr || j->first_attribute("tileHeight") == nullptr)
				continue;

			// Parse the values of the tileset
			std::string TilesetName = j->first_attribute("name")->value();
			std::string TilesetImage = WorkingDirectory + j->first_attribute("image")->value();
			unsigned int TileWidth = atoi(j->first_attribute("tileWidth")->value());
			unsigned int TileHeight = atoi(j->first_attribute("tileHeight")->value());

			// Verify that the tileset isn't a duplicate
			if (OgmoTilesets.find(TilesetName) != OgmoTilesets.end()){
				Warning(std::string("Tileset of name \"") + TilesetName + "\" already exists\n");
				continue;
			}

			// Prepare the tileset for storage
			terra::OgmoTileset NewTileset;
			NewTileset.Image = TilesetImage;
			NewTileset.TileWidth = TileWidth;
			NewTileset.TileHeight = TileHeight;

			// Store the tileset
			OgmoTilesets[TilesetName] = NewTileset;
		}
	}
}

void terra::Engine::RegisterObject(std::string Name, std::shared_ptr<terra::Item> (*Callback)(const terra::OgmoObject &)){
	if (Callbacks.find(Name) != Callbacks.end()){
		Warning(std::string("Object with name \"") + Name + "\" already registered\n");
		return;
	}
	Callbacks.insert(std::pair<std::string, std::shared_ptr<terra::Item> (*)(const terra::OgmoObject &)>(Name, Callback));
}

void terra::Engine::Warning(const std::string &WarningMessage){
	ConsoleLog.push_back(std::pair<unsigned int, std::string>(1, WarningMessage));
}

terra::Engine::~Engine(){
}

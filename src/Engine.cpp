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
	ConsoleOpen = false;
	Initialized = false;
}

void terra::Engine::Error(const std::string &ErrorMessage){
	ConsoleLog.push_back(std::pair<unsigned int, std::string>(2, ErrorMessage));
}

terra::Engine &terra::Engine::Get(){
	static terra::Engine Singleton;
	return Singleton;
}

std::shared_ptr<terra::Layer> terra::Engine::GetLayer(std::string Name){
	if (NamedLayers.find(Name) == NamedLayers.end())
		return std::shared_ptr<terra::Layer>(nullptr);
	return NamedLayers[Name];
}

sf::RenderWindow &terra::Engine::GetWindow(){
	return Window;
}

void terra::Engine::Initialize(const int argc, char *argv[]){
	unsigned int Width = 640, Height = 480, Framerate = 60;
	std::string Title = "Terra Game Engine", InitialLevel = "res/levels/Title.oel";
	ParseBoot(Width, Height, Framerate, Title, InitialLevel);
	ParseCommandLine(argc, argv, Width, Height);
	NewLevel = true;
	NextLevelName = InitialLevel;
	ParseProject();
	Window.Create(sf::VideoMode(Width, Height, 32), Title);
	Initialized = true;
}

const bool terra::Engine::IsConsoleOpen() const{
	return ConsoleOpen;
}

void terra::Engine::LoadLevel(std::string Filename){
	NewLevel = true;
	NextLevelName = Filename;
}

int terra::Engine::Main(){
	if (!Initialized){
		Error("Game attempted to run before initialization!\n");
		return -1;
	}
	sf::Font ConsoleFont;
	if (!ConsoleFont.LoadFromFile("res/fonts/DejaVuSansMono.ttf")){
		Warning("Unable to locate the console font. Going with default Arial.");
		ConsoleFont = sf::Font::GetDefaultFont();
	}
	while (Window.IsOpened()){
		if (!ConsoleOpen){
			if (NewLevel)
				ParseLevel();
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
			Window.Clear();
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnFrame();
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnRender(Window);
			Window.Display();
		}
		else{
			sf::Event Event;
			while (Window.PollEvent(Event)){
				if (Event.Type == sf::Event::Closed)
					Window.Close();
				else if (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape)
					ConsoleOpen = false;
			}
			sf::View Temp = Window.GetView();
			sf::View Replacement(sf::FloatRect(0, 0, Window.GetWidth(), Window.GetHeight()));
			Window.SetView(Replacement);
			Window.Clear();
			for (auto i = Layers.begin(); i != Layers.end(); ++i)
				for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
					(*j)->OnRender(Window);
			Window.Draw(sf::Shape::Rectangle(0., 0., Window.GetWidth(), Window.GetHeight(), sf::Color(0, 0, 0, 170)));
			unsigned int LineCounter = 0;
			unsigned int LineMax = Window.GetHeight()/20;
			for (auto i = ConsoleLog.rbegin(); i != ConsoleLog.rend(); ++i){
				if (LineCounter >= LineMax)
					break;
				unsigned int LineY = Window.GetHeight()-(LineCounter+1)*20;
				sf::Text Line(i->second, ConsoleFont, 16);
				if (i->first == 1)
					Line.SetColor(sf::Color::Yellow);
				else if (i->first == 2)
					Line.SetColor(sf::Color::Red);
				Line.SetX(2);
				Line.SetY(LineY);
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
	std::vector<char> Contents = terra::ReadFile("res/cfg/Boot.cfg");
	rapidxml::xml_document<> Boot;
	Boot.parse<0>(&Contents[0]);
	rapidxml::xml_node<> *Root = Boot.first_node("config");
	if (Root == nullptr || Root->type() != rapidxml::node_element){
		Error("Boot file \"res/cfg/Boot.cfg\" has no root node\n");
		return;
	}
	rapidxml::xml_node<> *WindowAttributes = Root->first_node("window");
	if (WindowAttributes != nullptr && WindowAttributes->type() == rapidxml::node_element){
		rapidxml::xml_node<> *WindowWidth = WindowAttributes->first_node("width");
		rapidxml::xml_node<> *WindowHeight = WindowAttributes->first_node("height");
		rapidxml::xml_node<> *WindowFramerate = WindowAttributes->first_node("framerate");
		rapidxml::xml_node<> *WindowTitle = WindowAttributes->first_node("title");
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
	if (Root->first_node("level") != nullptr && Root->first_node("level")->type() == rapidxml::node_element && Root->first_node("level")->first_node() != nullptr && Root->first_node("level")->first_node()->type() == rapidxml::node_data)
		InitialLevel = Root->first_node("level")->first_node()->value();
}

void terra::Engine::ParseCommandLine(const int argc, char *argv[], unsigned int &Width, unsigned int &Height){
	std::list<std::string> ArgumentList;
	for (int i = 1; i < argc; ++i)
		ArgumentList.push_back(argv[i]);
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
		if (Layers->type() != rapidxml::node_element)
			continue;
		// Parse Tile Layers
		for (auto LayerIterator = Layers->first_node("tiles"); LayerIterator != nullptr; LayerIterator = LayerIterator->next_sibling("tiles")){
			if (LayerIterator->type() != rapidxml::node_element)
				continue;
			ParseTileLayer(LayerIterator);
		}
		//! \todo Parse Object Layers
	}
}

void terra::Engine::ParseLevel(){
	for (auto i = Layers.begin(); i != Layers.end(); ++i)
		(*i)->Clear();
	NewLevel = false;
	std::vector<char> Contents = terra::ReadFile(NextLevelName);
	rapidxml::xml_document<> Level;
	Level.parse<0>(&Contents[0]);
	rapidxml::xml_node<> *LevelRoot = Level.first_node("level");
	if (LevelRoot == nullptr || LevelRoot->type() != rapidxml::node_element){
		Error(std::string("Level file \"") + NextLevelName + "\" has no root node\n");
		return;
	}
	for (auto i = NamedLayers.begin(); i != NamedLayers.end(); ++i){
		rapidxml::xml_node<> *LayerIterator = LevelRoot->first_node(i->first.c_str());
		if (LayerIterator == nullptr || LayerIterator->type() != rapidxml::node_element)
			continue;
		if (i->second->GetStoredType() == terra::Item::Tile)
			ParseLevelTileLayer(LayerIterator);
		//! \todo Parse Object Layers
	}
}

void terra::Engine::ParseLevelTileLayer(rapidxml::xml_node<> *TileLayer){
	if (TileLayer->first_attribute("set") == nullptr && !OgmoTileLayers[TileLayer->name()].MultipleTilesets)
		return;
	std::string Set;
	if (!OgmoTileLayers[TileLayer->name()].MultipleTilesets){
		Set = TileLayer->first_attribute("set")->value();
		if (OgmoTilesets.find(Set) == OgmoTilesets.end() || !GetTexture(Set))
			return;
	}
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
	for (auto TileIterator = TileLayer->first_node("tile"); TileIterator != nullptr; TileIterator = TileIterator->next_sibling("tile")){
		if (TileIterator->type() != rapidxml::node_element || TileIterator->first_attribute("x") == nullptr || TileIterator->first_attribute("y") == nullptr)
			continue;
		sf::Vector2f Position = sf::Vector2f(atof(TileIterator->first_attribute("x")->value()), atof(TileIterator->first_attribute("y")->value()));
		sf::Vector2<unsigned int> TilePosition;
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
		if (OgmoTileLayers[TileLayer->name()].ExportTileIDs){
			if (TileIterator->first_attribute("id") == nullptr)
				continue;
			unsigned int ID = atoi(TileIterator->first_attribute("id")->value());
			unsigned int IDWidth = (GetTexture(Set)->GetWidth()-GetTexture(Set)->GetWidth()%TileWidth)/TileWidth;
			unsigned int IDHeight = (GetTexture(Set)->GetHeight()-GetTexture(Set)->GetHeight()%TileHeight)/TileHeight;
			if (ID >= IDWidth*IDHeight)
				continue;
			TilePosition.x = ID%IDWidth*TileWidth;
			TilePosition.y = (ID-ID%IDWidth)/IDWidth*TileHeight;
		}
		else{
			if (TileIterator->first_attribute("tx") == nullptr || TileIterator->first_attribute("ty") == nullptr)
				continue;
			TilePosition.x = atoi(TileIterator->first_attribute("tx")->value());
			TilePosition.y = atoi(TileIterator->first_attribute("ty")->value());
		}
		terra::OgmoTile NextTile;
		NextTile.Tileset = Set;
		NextTile.TilePosition = TilePosition;
		NextTile.TileSize = sf::Vector2<unsigned int>(TileWidth, TileHeight);
		NextTile.Position = Position;
		NamedLayers[TileLayer->name()]->AddItem(std::shared_ptr<terra::Item>(new terra::Tile(NextTile)));
	}
}

void terra::Engine::ParseProject(){
	std::vector<char> Contents = terra::ReadFile("res/cfg/Levels.oep");
	rapidxml::xml_document<> Project;
	Project.parse<0>(&Contents[0]);
	rapidxml::xml_node<> *ProjectRoot = Project.first_node("project");
	if (ProjectRoot == nullptr || ProjectRoot->type() != rapidxml::node_element){
		Error("Project file \"res/cfg/Levels.oep\" has no root node\n");
		return;
	}
	ParseTilesets(ProjectRoot);
	//! \todo Parse Objects
	ParseLayers(ProjectRoot);
}

void terra::Engine::ParseTileLayer(rapidxml::xml_node<> *TileLayer){
	if (TileLayer->first_attribute("name") == nullptr)
		return;
	// Initial Properties
	std::string Name = TileLayer->first_attribute("name")->value();
	bool MultipleTilesets = false;
	bool ExportTileSize = false;
	bool ExportTileIDs = false;
	// Parse Nonessential Properties
	if (TileLayer->first_attribute("multipleTilesets") != nullptr && TileLayer->first_attribute("multipleTilesets")->value() == "true")
		MultipleTilesets = true;
	if (TileLayer->first_attribute("exportTileSize") != nullptr && TileLayer->first_attribute("exportTileSize")->value() == "true")
		ExportTileSize = true;
	if (TileLayer->first_attribute("exportTileIDs") != nullptr && TileLayer->first_attribute("exportTileIDs")->value() == "true")
		ExportTileIDs = true;
	// Check for redundancy
	if (NamedLayers.find(Name) != NamedLayers.end()){
		Error(std::string("Layer of name \"") + Name + "\" already exists\n");
		return;
	}
	// Create New Layer Data
	terra::OgmoTileLayer NewTileLayer;
	NewTileLayer.MultipleTilesets = MultipleTilesets;
	NewTileLayer.ExportTileSize = ExportTileSize;
	NewTileLayer.ExportTileIDs = ExportTileIDs;
	OgmoTileLayers[Name] = NewTileLayer;
	// Create New Layer
	std::shared_ptr<terra::Layer> NewLayer(new terra::Layer(terra::Item::Tile));
	NamedLayers[Name] = NewLayer;
	Layers.push_back(NewLayer);
}

void terra::Engine::ParseTilesets(rapidxml::xml_node<> *Root){
	std::string WorkingDirectory = "res/cfg/";
	for (auto Settings = Root->first_node("settings"); Settings != nullptr; Settings = Settings->next_sibling("settings")){
		if (Settings->type() != rapidxml::node_element)
			continue;
		bool Found = false;
		for (auto WorkingDirectoryNode = Settings->first_node("workingDirectory"); WorkingDirectoryNode != nullptr; WorkingDirectoryNode = WorkingDirectoryNode->next_sibling("workingDirectory")){
			if (WorkingDirectoryNode->type() != rapidxml::node_element || WorkingDirectoryNode->first_node() == nullptr || WorkingDirectoryNode->first_node()->type() != rapidxml::node_data)
				continue;
			WorkingDirectory += WorkingDirectoryNode->first_node()->value();
			Found = true;
			break;
		}
		if (Found)
			break;
	}
	for (auto i = Root->first_node("tilesets"); i != nullptr; i = i->next_sibling("tilesets")){
		if (i->type() != rapidxml::node_element)
			continue;
		for (auto j = i->first_node("tileset"); j != nullptr; j = j->next_sibling("tileset")){
			if (j->type() != rapidxml::node_element || j->first_attribute("name") == nullptr || j->first_attribute("image") == nullptr || j->first_attribute("tileWidth") == nullptr || j->first_attribute("tileHeight") == nullptr)
				continue;
			std::string TilesetName = j->first_attribute("name")->value();
			std::string TilesetImage = WorkingDirectory + j->first_attribute("image")->value();
			unsigned int TileWidth = atoi(j->first_attribute("tileWidth")->value());
			unsigned int TileHeight = atoi(j->first_attribute("tileHeight")->value());
			if (OgmoTilesets.find(TilesetName) != OgmoTilesets.end()){
				Warning(std::string("Tileset of name \"") + TilesetName + "\" already exists\n");
				continue;
			}
			terra::OgmoTileset NewTileset;
			NewTileset.Image = TilesetImage;
			NewTileset.TileWidth = TileWidth;
			NewTileset.TileHeight = TileHeight;
			OgmoTilesets[TilesetName] = NewTileset;
		}
	}
}

/*void terra::Engine::RegisterObject(std::string Name, std::shared_ptr<terra::Object> (*Callback)(const terra::OgmoObject &)){
	if (Callbacks.find(Name) != Callbacks.end()){
		Warning(std::string("Object with name \"") + Name + "\" already registered\n");
		return;
	}
	Callbacks.insert(std::pair<std::string, std::shared_ptr<terra::Object> (*)(const terra::OgmoObject &)>(Name, Callback));
}*/

void terra::Engine::Warning(const std::string &WarningMessage){
	ConsoleLog.push_back(std::pair<unsigned int, std::string>(1, WarningMessage));
}

terra::Engine::~Engine(){
}

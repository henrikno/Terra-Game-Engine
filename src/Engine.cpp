#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include "Engine.hpp"

terra::Engine::Engine(){
	Initialized = false;
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
	std::string Title = "Terra Game Engine";
	std::ifstream FileStream("res/cfg/Boot.cfg");
	FileStream.seekg(0, std::ios::end);
	int Length = FileStream.tellg();
	FileStream.seekg(0, std::ios::beg);
	char *Buffer = new char[Length+1];
	Buffer[Length] = 0;
	FileStream.read(Buffer, Length);
	if (!FileStream.good()){
		std::cerr << "Error: Unexpected error occured when reading \"res/cfg/Boot.cfg\"\n";
		FileStream.close();
		return;
	}
	FileStream.close();
	std::string String = Buffer;
	delete[] Buffer;
	rapidxml::xml_document<> Boot;
	Boot.parse<0>(&String.at(0));
	rapidxml::xml_node<> *Root = Boot.first_node("config");
	if (Root == nullptr || Root->type() != rapidxml::node_element){
		std::cerr << "Error: No root node found in \"res/cfg/Boot.cfg\"\n";
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
	NewLevel = true;
	NextLevelName = "res/levels/Title.oel";
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
		else
			std::cerr << "Invalid option on command line!\n";
	}
	ParseProject();
	Window.Create(sf::VideoMode(Width, Height, 32), Title);
	Initialized = true;
}

void terra::Engine::LoadLevel(std::string Filename){
	NewLevel = true;
	NextLevelName = Filename;
}

int terra::Engine::Main(){
	if (!Initialized){
		std::cerr << "Game attempted to run before initialization!\n";
		return -1;
	}
	while (Window.IsOpened()){
		if (NewLevel)
			ParseLevel();
		sf::Event Event;
		while (Window.PollEvent(Event)){
			if (Event.Type == sf::Event::Closed)
				Window.Close();
			else{
				for (auto i = Layers.begin(); i != Layers.end(); ++i){
					for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
						(*j)->OnEvent(Event);
				}
			}
		}
		for (auto i = Layers.begin(); i != Layers.end(); ++i){
			for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
				(*j)->OnFrame();
		}
		for (auto i = Layers.begin(); i != Layers.end(); ++i){
			for (auto j = (*i)->Begin(); j != (*i)->End(); ++j)
				(*j)->OnRender(Window);
		}
		Window.Display();
	}
	return 0;
}

void terra::Engine::ParseLevel(){
	for (auto i = NamedLayers.begin(); i != NamedLayers.end(); ++i)
		i->second->Clear();
	if (!NewLevel)
		return;
	NewLevel = false;
	std::ifstream FileStream(NextLevelName.c_str());
	FileStream.seekg(0, std::ios::end);
	int Length = FileStream.tellg();
	FileStream.seekg(0, std::ios::beg);
	char *Buffer = new char[Length+1];
	Buffer[Length] = 0;
	FileStream.read(Buffer, Length);
	if (!FileStream.good()){
		std::cerr << "Error: Unexpected error occured when reading \"" << NextLevelName << "\"\n";
		FileStream.close();
		return;
	}
	FileStream.close();
	std::string String = Buffer;
	delete[] Buffer;
	rapidxml::xml_document<> Level;
	Level.parse<0>(&String.at(0));
	rapidxml::xml_node<> *Root = Level.first_node("level");
	if (Root == nullptr || Root->type() != rapidxml::node_element){
		std::cerr << "Error: No root node found in Ogmo Editor Level \"" << NextLevelName << "\"\n";
		return;
	}
	rapidxml::xml_node<> *LayerIterator;
	for (auto i = NamedLayers.begin(); i != NamedLayers.end(); ++i){
		LayerIterator = Root->first_node(i->first.c_str());
		if (LayerIterator == nullptr)
			continue;
		//! \todo Parse Grid Layers
		//! \todo Parse Tile Layers
		if (i->second->GetStoredType() == terra::Item::Object){
			for (auto j = LayerIterator->first_node(); j != nullptr; j = j->next_sibling()){
				if (j->type() != rapidxml::node_element)
					continue;
				if (OgmoObjects.find(j->name()) == OgmoObjects.end()){
					std::cerr << "Error: No object of name \"" << j->name() << "\" exists in the Ogmo Project\n";
					continue;
				}
				terra::OgmoObject NewObject = OgmoObjects[j->name()];
				for (auto k = j->first_attribute(); k != nullptr; k = k->next_attribute())
					NewObject.Attributes[k->name()] = k->value();
				if (Callbacks.find(j->name()) == Callbacks.end()){
					std::cerr << "Error: No object of name \"" << j->name() << "\" is registered in the engine\n";
					continue;
				}
				i->second->AddItem(Callbacks[j->name()](NewObject));
			}
		}
	}
}

void terra::Engine::ParseObjectFolder(rapidxml::xml_node<> *Folder){
	if (Folder->type() != rapidxml::node_element)
		return;
	for (rapidxml::xml_node<> *FolderIterator = Folder->first_node("object"); FolderIterator != nullptr; FolderIterator = FolderIterator->next_sibling("object")){
		if (FolderIterator->type() != rapidxml::node_element)
			continue;
		terra::OgmoObject NewObject;
		rapidxml::xml_node<> *Values = FolderIterator->first_node("values");
		if (Values != nullptr && Values->type() == rapidxml::node_element){
			for (rapidxml::xml_node<> *ValueIterator = Values->first_node(); ValueIterator != nullptr; ValueIterator = ValueIterator->next_sibling()){
				if (ValueIterator->type() != rapidxml::node_element || ValueIterator->name() != "boolean" || ValueIterator->name() != "integer" || ValueIterator->name() != "number" || ValueIterator->name() != "string" || ValueIterator->name() != "text")
					continue;
				std::string AttributeName;
				std::string DefaultValue;
				rapidxml::xml_attribute<> *Name = ValueIterator->first_attribute("name");
				rapidxml::xml_attribute<> *Default = ValueIterator->first_attribute("default");
				if (Name == nullptr || Default == nullptr)
					continue;
				AttributeName = Name->value();
				DefaultValue = Default->value();
				NewObject.Attributes[AttributeName] = DefaultValue;
			}
		}
		OgmoObjects.insert(std::pair<std::string, terra::OgmoObject>(FolderIterator->first_attribute("name")->value(), NewObject));
	}
	for (rapidxml::xml_node<> *FolderIterator = Folder->first_node("folder"); FolderIterator != nullptr; FolderIterator = FolderIterator->next_sibling("folder"))
		ParseObjectFolder(FolderIterator);
}

void terra::Engine::ParseProject(){
	std::ifstream FileStream("res/cfg/Project.oep");
	FileStream.seekg(0, std::ios::end);
	int Length = FileStream.tellg();
	FileStream.seekg(0, std::ios::beg);
	char *Buffer = new char[Length+1];
	Buffer[Length] = 0;
	FileStream.read(Buffer, Length);
	if (!FileStream.good()){
		std::cerr << "Error: Unexpected error occured when reading \"res/cfg/Project.oep\"\n";
		FileStream.close();
		return;
	}
	FileStream.close();
	std::string String = Buffer;
	delete[] Buffer;
	rapidxml::xml_document<> Project;
	Project.parse<0>(&String.at(0));
	rapidxml::xml_node<> *Root = Project.first_node("project");
	if (Root == nullptr || Root->type() != rapidxml::node_element){
		std::cerr << "Error: No root node found in Ogmo Editor Project \"res/cfg/Project.oep\"\n";
		return;
	}
	// Parse Objects
	for (rapidxml::xml_node<> *ProjectIterator = Root->first_node("objects"); ProjectIterator != nullptr; ProjectIterator = ProjectIterator->next_sibling("objects"))
		ParseObjectFolder(ProjectIterator);
	// Parse Layers
	for (rapidxml::xml_node<> *ProjectIterator = Root->first_node("layers"); ProjectIterator != nullptr; ProjectIterator = ProjectIterator->next_sibling("layers")){
		if (ProjectIterator->type() != rapidxml::node_element)
			continue;
		for (rapidxml::xml_node<> *LayerIterator = ProjectIterator->first_node(); LayerIterator != nullptr; LayerIterator = LayerIterator->next_sibling()){
			if (LayerIterator->type() == rapidxml::node_element && LayerIterator->name() == "grid"){
				//! \todo Implement Ogmo Grids
				std::cerr << "Warning: Grids are not supported at this time\n";
				continue;
			}
			else if (LayerIterator->type() == rapidxml::node_element && LayerIterator->name() == "tiles"){
				//! \todo Implement Ogmo Tiles
				std::cerr << "Warning: Tiles are not supported at this time\n";
				continue;
			}
			else if (LayerIterator->type() == rapidxml::node_element && LayerIterator->name() == "objects"){
				NamedLayers.insert(std::pair<std::string, std::shared_ptr<terra::Layer>>(LayerIterator->first_attribute("name")->value(), std::shared_ptr<terra::Layer>(new terra::Layer(terra::Item::Object))));
				Layers.push_back(NamedLayers[LayerIterator->first_attribute("name")->value()]);
			}
		}
	}
}

void terra::Engine::RegisterObject(std::string Name, std::shared_ptr<terra::Object> (*Callback)(const terra::OgmoObject &)){
	if (Callbacks.find(Name) != Callbacks.end()){
		std::cerr << "Object with name \"" << Name << "\" already registered\n";
		return;
	}
	Callbacks.insert(std::pair<std::string, std::shared_ptr<terra::Object> (*)(const terra::OgmoObject &)>(Name, Callback));
}

terra::Engine::~Engine(){
}

#include <iostream>
#include "Engine.hpp"

terra::Engine &terra::Engine::Get(){
	static terra::Engine Instance;
	return Instance;
}

terra::Engine::Engine(){
	NewLevel = false;
	Ready = false;
}

std::multimap<int, std::shared_ptr<terra::Object>> &terra::Engine::GetObjects(){
	return ObjectList;
}

const int terra::Engine::GetOpenGLMajorVersion() const{
	return GameWindow.GetSettings().MajorVersion;
}

const int terra::Engine::GetOpenGLMinorVersion() const{
	return GameWindow.GetSettings().MinorVersion;
}

sf::RenderWindow &terra::Engine::GetWindow(){
	return GameWindow;
}

void terra::Engine::Initialize(int argc, char *argv[]){
	std::vector<std::string> Args;
	for (int i = 0; i < argc; ++i){
		std::string Argument = argv[i];
		Args.push_back(Argument);
	}
	int16_t Width = 0, Height = 0, Framerate = 0;
	std::string Title = "", InitialLevelFileName = "";
	nbt::File ConfigFile("res/config/core.dat");
	std::shared_ptr<nbt::Compound> Root = ConfigFile.GetRoot();
	if (!Root){
		std::cerr << "Failed to load configuration file res/config/core.dat\n";
		return;
	}
	auto WindowPtr = Root->FirstChild("Window");
	if (WindowPtr != Root->GetPayload().end() && (*WindowPtr)->GetType() == 10){
		nbt::Compound *WindowCompound = reinterpret_cast<nbt::Compound*>(WindowPtr->get());
		auto WidthPtr = WindowCompound->FirstChild("Width");
		if (WidthPtr != Root->GetPayload().end() && (*WidthPtr)->GetType() == 2)
			Width = reinterpret_cast<nbt::Short*>(WidthPtr->get())->GetPayload();
		auto HeightPtr = WindowCompound->FirstChild("Height");
		if (HeightPtr != Root->GetPayload().end() && (*HeightPtr)->GetType() == 2)
			Height = reinterpret_cast<nbt::Short*>(HeightPtr->get())->GetPayload();
		auto FrameratePtr = WindowCompound->FirstChild("Framerate");
		if (FrameratePtr != Root->GetPayload().end() && (*FrameratePtr)->GetType() == 2)
			Framerate = reinterpret_cast<nbt::Short*>(FrameratePtr->get())->GetPayload();
		auto TitlePtr = WindowCompound->FirstChild("Title");
		if (TitlePtr != Root->GetPayload().end() && (*TitlePtr)->GetType() == 8)
			Title = reinterpret_cast<nbt::String*>(TitlePtr->get())->GetPayload();
	}
	auto LevelPtr = Root->FirstChild("Level");
	if (LevelPtr != Root->GetPayload().end() && (*LevelPtr)->GetType() == 8)
		InitialLevelFileName = reinterpret_cast<nbt::String*>(LevelPtr->get())->GetPayload();
	for (auto i = Args.begin(); i != Args.end(); ++i){
		if (*i == "-width")
			Width = atoi((++i)->c_str());
		else if (*i == "-height")
			Height = atoi((++i)->c_str());
		else if (*i == "-framerate")
			Framerate = atoi((++i)->c_str());
		else if (*i == "-title")
			Title = *(++i);
		else if (*i == "-level")
			InitialLevelFileName = *(++i);
	}
	if (Width > 0 && Height > 0 && InitialLevelFileName.size() > 0){
		Ready = true;
		NewLevel = true;
		NewLevelName = InitialLevelFileName;
		GameWindow.Create(sf::VideoMode(Width, Height, 32), Title, sf::Style::Default, sf::ContextSettings(24, 8, 0, 4, 1));
		GameWindow.SetFramerateLimit(Framerate);
	}
	else
		std::cerr << "Failed to load one or more of the following from res/config/core.dat and/or command line options:\nwidth\nheight\ninitial level filename\n";
}

void terra::Engine::LoadLevel(std::string Filename){
	if (Filename.size() == 0)
		return;
	NewLevel = true;
	NewLevelName = Filename;
}

int terra::Engine::Main(){
	if (!Ready){
		std::cerr << "Initialization failure\n";
		return 1;
	}
	while (GameWindow.IsOpened()){
		if (NewLevel)
			PrepareLevel();
		sf::Event Event;
		while (GameWindow.PollEvent(Event)){
			for (auto i = ObjectList.begin(); i != ObjectList.end(); ++i)
				i->second->OnEvent(Event);
			if (Event.Type == sf::Event::Closed)
				GameWindow.Close();
		}
		for (auto i = ObjectList.begin(); i != ObjectList.end(); ++i)
			i->second->OnFrame();
		GameWindow.Clear();
		for (auto i = ObjectList.begin(); i != ObjectList.end(); ++i)
			i->second->OnRender(GameWindow);
		GameWindow.Display();
	}
	return 0;
}

void terra::Engine::PrepareLevel(){
	NewLevel = false;
	nbt::File LevelFile(NewLevelName);
	std::shared_ptr<nbt::Compound> Root = LevelFile.GetRoot();
	if (!Root){
		std::cerr << "Unable to load level " << NewLevelName << '\n';
		return;
	}
	for (auto i = Root->FirstChild("Layer"); i != Root->GetPayload().end(); i = Root->NextChild("Layer", i)){
		if ((*i)->GetType() != 10)
			continue;
		nbt::Compound *Layer = reinterpret_cast<nbt::Compound*>(i->get());
		int LayerDepth = 0;
		auto j = Layer->FirstChild("Depth");
		if (j == Layer->GetPayload().end() || (*j)->GetType() != 3){
			std::cerr << "No depth found for layer, ignoring\n";
			continue;
		}
		LayerDepth = reinterpret_cast<nbt::Int*>(j->get())->GetPayload();
		j = Layer->FirstChild("Objects");
		if (j == Layer->GetPayload().end() || (*j)->GetType() != 9 || reinterpret_cast<nbt::List*>(j->get())->GetTagType() != 10){
			std::cerr << "No objects found for layer, ignoring\n";
			continue;
		}
		nbt::List *Objects = reinterpret_cast<nbt::List*>(j->get());
		for (auto k = Objects->GetPayload().begin(); k != Objects->GetPayload().end(); ++k){
			auto Type = reinterpret_cast<nbt::Compound*>(k->get())->FirstChild("Type");
			if (Type == reinterpret_cast<nbt::Compound*>(k->get())->GetPayload().end() || (*Type)->GetType() != 8){
				std::cerr << "No type found for object, ignoring\n";
				continue;
			}
			if (ObjectCallbacks.find(reinterpret_cast<nbt::String*>(Type->get())->GetPayload()) == ObjectCallbacks.end()){
				std::cerr << "Unknown object type " << reinterpret_cast<nbt::String*>(Type->get())->GetPayload() << ", ignoring\n";
				continue;
			}
			auto Data = reinterpret_cast<nbt::Compound*>(k->get())->FirstChild("Data");
			if (Data == reinterpret_cast<nbt::Compound*>(k->get())->GetPayload().end() || (*Data)->GetType() != 10){
				std::cerr << "No data found for object, ignoring\n";
				continue;
			}
			ObjectList.insert(std::pair<int, std::shared_ptr<terra::Object>>(LayerDepth, ObjectCallbacks[reinterpret_cast<nbt::String*>(Type->get())->GetPayload()](reinterpret_cast<nbt::Compound*>(Data->get()))));
		}
	}
}

void terra::Engine::Register(std::string Name, std::shared_ptr<terra::Object> (*CallbackConstructor)(nbt::Compound*)){
	if (ObjectCallbacks.find(Name) != ObjectCallbacks.end()){
		std::cerr << "Object type " << Name << " already registered, ignoring\n";
		return;
	}
	ObjectCallbacks.insert(std::pair<std::string, std::shared_ptr<terra::Object> (*)(nbt::Compound*)>(Name, CallbackConstructor));
}

terra::Engine::~Engine(){
}

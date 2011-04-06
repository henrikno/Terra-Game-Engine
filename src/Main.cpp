#include "Engine.hpp"

int main(int argc, char *argv[]){
	terra::Engine::Get().Initialize(argc, argv);
	// Insert all object registration at this point
	return terra::Engine::Get().Main();
}

#include "Engine.hpp"

int main(int argc, char *argv[]){
	terra::Engine::Get().Initialize(argc, argv);
	// Insert all Object Registration Here
	return terra::Engine::Get().Main();
}


#include <chrono>
#include <ostream>

#include "ECS/ECS.h"
#include "Engine/Engine.h"

int main(int argc, char* argv[])
{
	ECSEngine::Engine engine(1920, 1080);
	
	engine.Initialize();
	engine.Run();
	engine.Destroy();
	
	return 0;
}
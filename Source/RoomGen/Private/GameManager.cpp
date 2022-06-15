// Nikita Sergeev

#include "GameManager.h"
#include "JSONParser.h"
#include "RoomGenerator.h"

AGameManager::AGameManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
    // 1. Init an Actor
    // 2. JSON File Attempt to open
    // 3. Parse Data from JSON to a struct
    // 4. Create a room based on the input as an Actor
	// 5. Spawn Doors/Windows

	// JSON to Struct parsing
	FString FullPath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir());
	const FString Filename = "building_test.json";
	const FString FullFilepath = FPaths::Combine(FullPath, Filename);
	
	const JSONParser JsonParser = JSONParser(FullFilepath);
	TArray<FWall> Walls;
	const FString Str = JsonParser.Parse(Walls) ?
		"JSON Successfully parsed" : "JSON was Not parsed";

	// Building Mesh
	auto* RoomGenerator = GetWorld()->SpawnActor<ARoomGenerator>();
	RoomGenerator->GenerateRoom(Walls);
	
#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *Str));
	// UE_LOG(LogTemp, Warning, TEXT("PATH: %s"), *Str);
#endif
}
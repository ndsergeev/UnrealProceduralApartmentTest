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
	
    // 1. JSON File Attempt to open
    // 2. Parse Data from JSON to a struct
    // 3. Create a room based on the input
    // 4. Spawn an Actor

	// JSON to Struct parsing
	FString FullPath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir());
	const FString Filename = "building_test.json";
	const FString FullFilepath = FPaths::Combine(FullPath, Filename);
	
	const JSONParser JsonParser = JSONParser(FullFilepath);
	TArray<FWall> Walls;
	const FString Str = JsonParser.Parse(Walls) ?
		"JSON Successfully generated" : "JSON was not generated";

	// Building Mesh
	auto* RoomGenerator = GetWorld()->SpawnActor<ARoomGenerator>();
	RoomGenerator->GenerateCube(Walls);
	
#ifdef UE_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *Str));
	// UE_LOG(LogTemp, Warning, TEXT("PATH: %s"), *Str);
#endif
}
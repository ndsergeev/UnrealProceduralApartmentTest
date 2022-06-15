// Nikita Sergeev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"

#include "FWall.h"
#include "RoomGenerator.generated.h"


UCLASS()
class ROOMGEN_API ARoomGenerator final : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomGenerator();

protected:
	FDynamicMesh3 *TargetMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMesh *WindowMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMesh *DoorMesh;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> Windows;
	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> Doors;

	UMaterial *Material;
	
	TArray<uint32> Triangles;
	TArray<FVector2D> UVCoords;
	TArray<FVector> Normals;
	
public:	
	UPROPERTY(VisibleAnywhere)
	UDynamicMeshComponent* MeshComponent;
	
	FDynamicMesh3 GenerateCube(const FKube &Transform);
	void GenerateRoom(const TArray<FWall> &Walls);
	void SpawnWindow(const FKube &Transform);
	void SpawnDoor(const FKube &Transform);
};

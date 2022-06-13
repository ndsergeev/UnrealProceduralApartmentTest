// Nikita Sergeev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
// #include "DynamicMesh/DynamicMeshAttributeSet.h"
// #include "DynamicMeshEditor.h"

// this should not be used...
#include "ProceduralMeshComponent.h"

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
	
	TArray<uint32> Triangles;
	TArray<FVector2D> UVCoords;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	
public:	
	UPROPERTY(VisibleAnywhere)
	// UProceduralMeshComponent* MeshComponent;
	UDynamicMeshComponent* MeshComponent;
	UMaterial *Material;
	
	FDynamicMesh3 GenerateCube(const FKube &Transform);
	void GenerateRoom(const TArray<FWall> &Walls);
};

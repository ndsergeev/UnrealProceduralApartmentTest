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
	UE::Geometry::FDynamicMesh3 *TargetMesh;
	
public:	
	UPROPERTY(VisibleAnywhere)
	// UProceduralMeshComponent* MeshComponent;
	UDynamicMeshComponent* MeshComponent;

	// this is for DynamicMesh3, it requires a different type
	TArray<uint32> Triangles;
	TArray<FVector2D> UVCoords;
	TArray<FVector> Normals;
	
    TArray<FProcMeshTangent> Tangents;

	UMaterial *Material;
	
	FDynamicMesh3 GenerateCube(const FKube &Transform);
	void GenerateRoom(const TArray<FWall> &Walls);
};

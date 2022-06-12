// Nikita Sergeev

#include "RoomGenerator.h"
#include "KismetProceduralMeshLibrary.h"

ARoomGenerator::ARoomGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Dynamic Mesh Component"));
	MeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("Dynamic Mesh Component"));

	for (const auto v : {
		6, 1, 0,
		7, 6, 0,
		6, 7, 5,
		7, 4, 5,
		3, 4, 0,
		4, 7, 0,
		6, 5, 1,
		5, 2, 1,
		5, 4, 2,
		4, 3, 2,
		2, 3, 0,
		1, 2, 0
	})
	{
		Triangles.Add(v);
	}
	
	UVCoords.Add(FVector2D(	 0.5f,		0));
	UVCoords.Add(FVector2D(	0.25f,		0));
	UVCoords.Add(FVector2D(	 0.5f,		1));
	UVCoords.Add(FVector2D(	0.25f,		1));
	UVCoords.Add(FVector2D(		1,	0.33f));
	UVCoords.Add(FVector2D(		1,	0.66f));
	UVCoords.Add(FVector2D(	0.75f,	0.33f));
	UVCoords.Add(FVector2D(	0.75f,	0.66f));
	UVCoords.Add(FVector2D(	 0.5f,	0.33f));
	UVCoords.Add(FVector2D(	 0.5f,	0.66f));
	UVCoords.Add(FVector2D(	0.25f,	0.33f));
	UVCoords.Add(FVector2D(		0,	0.33f));
	UVCoords.Add(FVector2D(	0.25f,	0.66f));
	UVCoords.Add(FVector2D(		0,	0.66f));
}

void ARoomGenerator::GenerateCube(const TArray<FWall> &Walls)
{
// #ifdef UE_EDITOR
// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("GenerateCube, Have read the Struct, size: %i"), Walls.Num()));
// #endif
//
// 	// TODO: this should be replaced with dynamic mesh...
// 	TArray<FVector> Vertices;
// 	Vertices.Add(FVector(0,   0,     0)); // 0
// 	Vertices.Add(FVector(100, 0,     0)); // 1
// 	Vertices.Add(FVector(100, 100,   0)); // 2
// 	Vertices.Add(FVector(0,   100,   0)); // 3
// 	Vertices.Add(FVector(0,   100, 100)); // 4
// 	Vertices.Add(FVector(100, 100, 100)); // 5
// 	Vertices.Add(FVector(100, 0,   100)); // 6
// 	Vertices.Add(FVector(0,   0,   100)); // 7
// 	
// 	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVCoords, Normals, Tangents);
// 	MeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UVCoords, TArray<FColor>(), Tangents, false);
//
//
// 	auto Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
// 	Path = FPaths::Combine(Path, "/StarterContent/Materials/DefaultMaterial");
// 	Material = LoadObject<UMaterial>(nullptr, *Path);
// 	
// 	if (Material != nullptr)
// 	{
// 		MeshComponent->SetMaterial(0, Material);
// 	}
// #ifdef UE_EDITOR
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("IT DOES NOT WORK")));
// 		// UE_LOG(LogTemp, Warning, TEXT("PATH: %s"), *Str);
// 	}
// #endif
// 	
	//TODO: I wish I can make the line below work...
	UE::Geometry::FDynamicMesh3 DynamicMesh3;
	DynamicMesh3.AppendVertex(FVector3d(0,   0,     0)); // 0
	DynamicMesh3.AppendVertex(FVector3d(100, 0,     0)); // 1
	DynamicMesh3.AppendVertex(FVector3d(100, 100,   0)); // 2
	DynamicMesh3.AppendVertex(FVector3d(0,   100,   0)); // 3
	DynamicMesh3.AppendVertex(FVector3d(0,   100, 100)); // 4
	DynamicMesh3.AppendVertex(FVector3d(100, 100, 100)); // 5
	DynamicMesh3.AppendVertex(FVector3d(100, 0,   100)); // 6
	DynamicMesh3.AppendVertex(FVector3d(0,   0,   100)); // 7
	
	for (UE::Geometry::FIndex3i v : {
		UE::Geometry::FIndex3i(6, 1, 0),
		UE::Geometry::FIndex3i(7, 6, 0),
		UE::Geometry::FIndex3i(6, 7, 5),
		UE::Geometry::FIndex3i(7, 4, 5),
		UE::Geometry::FIndex3i(3, 4, 0),
		UE::Geometry::FIndex3i(4, 7, 0),
		UE::Geometry::FIndex3i(6, 5, 1),
		UE::Geometry::FIndex3i(5, 2, 1),
		UE::Geometry::FIndex3i(5, 4, 2),
		UE::Geometry::FIndex3i(4, 3, 2),
		UE::Geometry::FIndex3i(2, 3, 0),
		UE::Geometry::FIndex3i(1, 2, 0)
	})
	{
		DynamicMesh3.AppendTriangle(v);
	}

	MeshComponent->SetMesh(MoveTemp(DynamicMesh3));
}

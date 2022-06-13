// Nikita Sergeev

#include "RoomGenerator.h"
#include "KismetProceduralMeshLibrary.h"
#include "Operations/MeshBoolean.h"

ARoomGenerator::ARoomGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	TargetMesh = new FDynamicMesh3;
	
	MeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("Dynamic Mesh Component"));
}

FDynamicMesh3 ARoomGenerator::GenerateCube(const FKube &Transform)
{
	UE::Geometry::FDynamicMesh3 DynamicMesh3;
	DynamicMesh3.AppendVertex(FVector3d(Transform.X,    Transform.Y,       Transform.Z)); // 0
	DynamicMesh3.AppendVertex(FVector3d(Transform.XOff, Transform.Y,       Transform.Z)); // 1
	DynamicMesh3.AppendVertex(FVector3d(Transform.XOff, Transform.YOff,    Transform.Z)); // 2
	DynamicMesh3.AppendVertex(FVector3d(Transform.X,    Transform.YOff,    Transform.Z)); // 3
	DynamicMesh3.AppendVertex(FVector3d(Transform.X,    Transform.YOff, Transform.ZOff)); // 4
	DynamicMesh3.AppendVertex(FVector3d(Transform.XOff, Transform.YOff, Transform.ZOff)); // 5
	DynamicMesh3.AppendVertex(FVector3d(Transform.XOff, Transform.Y,    Transform.ZOff)); // 6
	DynamicMesh3.AppendVertex(FVector3d(Transform.X,    Transform.Y,    Transform.ZOff)); // 7
	
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

	return DynamicMesh3;
}

void ARoomGenerator::GenerateRoom(const TArray<FWall> &Walls)
{
	// Generate Walls
	 for (const auto & [Transform, Subtract] : Walls)
	 {
	 	FDynamicMesh3 MeshA, MeshB;
	 	MeshA.Copy(*TargetMesh);
	 	MeshB = GenerateCube(Transform);
	 	
	 	for (const auto &SubTransform : Subtract)
	 	{
	 		auto MeshSub = GenerateCube(SubTransform);
	 		auto SubBool = UE::Geometry::FMeshBoolean(&MeshB, &MeshSub, &MeshB,
	 		UE::Geometry::FMeshBoolean::EBooleanOp::Difference);
	 		SubBool.Compute();
	 	}
	 	
		 auto UniBool = UE::Geometry::FMeshBoolean(&MeshA, &MeshB, TargetMesh,
		 	UE::Geometry::FMeshBoolean::EBooleanOp::Union);
	 	UniBool.Compute();
	 }
	
	MeshComponent->SetMesh(MoveTemp(*TargetMesh));

	auto Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	Path = FPaths::Combine(Path, "/StarterContent/Materials/DefaultMaterial");
	Material = LoadObject<UMaterial>(nullptr, *Path);

	if (Material != nullptr)
	{
		MeshComponent->SetMaterial(0, Material);
	}
}

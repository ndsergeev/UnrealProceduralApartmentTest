// Nikita Sergeev

#include "RoomGenerator.h"
#include "Operations/MeshBoolean.h"
#include "Operations/PolygroupRemesh.h"
#include "ConstrainedDelaunay2.h"

ARoomGenerator::ARoomGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	TargetMesh = new FDynamicMesh3(true, true, true, false);
	MeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(TEXT("Dynamic Mesh Component"));
	MeshComponent->SetCastShadow(false);

	auto Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	const auto WindowPath = FPaths::Combine(Path, "StaticMesh'/Game/StarterContent/Meshes/Window.Window'");
	// if (FPaths::FileExists(WindowPath))
	{
		WindowMesh = LoadObject<UStaticMesh>(nullptr, *WindowPath);
	}
	
	const auto DoorPath = FPaths::Combine(Path, "StaticMesh'/Game/StarterContent/Meshes/Door.Door'");
	// if (FPaths::FileExists(DoorPath))
	{
		DoorMesh = LoadObject<UStaticMesh>(nullptr, *DoorPath);	
	}
}

FDynamicMesh3 ARoomGenerator::GenerateCube(const FKube &Transform)
{
	FDynamicMesh3 DynamicMesh3;
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
	// Generate Walls, probably FMeshSelfUnion could be used...
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

			switch (SubTransform.Type)
			{
				case 1: SpawnWindow(SubTransform);
					break;
				case 2: SpawnDoor(SubTransform);
					break;
			}
		}
	 	
		auto UniBool = UE::Geometry::FMeshBoolean(&MeshA, &MeshB, TargetMesh,
			UE::Geometry::FMeshBoolean::EBooleanOp::Union);
		UniBool.Compute();
	}
	
	if (!TargetMesh->CheckValidity())
	{
		UE::Geometry::FGroupTopology Topology(TargetMesh, true);
		UE::Geometry::FPolygroupRemesh Remesh(TargetMesh, &Topology, UE::Geometry::ConstrainedDelaunayTriangulate<double>);
		Remesh.Compute();
	}

	MeshComponent->SetMobility(EComponentMobility::Stationary);
	MeshComponent->SetMesh(MoveTemp(*TargetMesh));
	
	auto Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	Path = FPaths::Combine(Path, "Material'/Game/StarterContent/Materials/DefaultMaterial.DefaultMaterial'");
	Material = LoadObject<UMaterial>(nullptr, *Path);

	if (Material != nullptr)
	{
		MeshComponent->SetMaterial(0, Material);
	}
}

/**
 * @brief It was a very simple 1-min-model made in blender that I
 * stretch based Subtract shape.
 * @param Transform - Subtract in JSON with Type '1'
 */
void ARoomGenerator::SpawnWindow(const FKube &Transform)
{
	if (WindowMesh == nullptr)
		return;

	// I know it is better to cache it in the class declaration, I trust compiler...
	constexpr float WindowSize = 20;
	
	UStaticMeshComponent* TmpWindowComp = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	TmpWindowComp->RegisterComponent();
	TmpWindowComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	TmpWindowComp->SetWorldLocation(Transform.GetCenter());

	const auto Rot = FQuat::MakeFromEuler(FVector(0, 0, Transform.Rot));
	TmpWindowComp->SetWorldRotation(Rot);

	TmpWindowComp->SetWorldScale3D(Transform.GetFrame(WindowSize));
	TmpWindowComp->SetVisibility(true);

	TmpWindowComp->Mobility = EComponentMobility::Stationary;
	// TmpWindowComp->Mobility = EComponentMobility::Movable;
	TmpWindowComp->bUseDefaultCollision = false;
	TmpWindowComp->SetStaticMesh(WindowMesh);

	Windows.Add(TmpWindowComp);
}

void ARoomGenerator::SpawnDoor(const FKube &Transform)
{
	if (DoorMesh == nullptr)
		return;
	
	UStaticMeshComponent* TmpDoorComp = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	TmpDoorComp->RegisterComponent();
	TmpDoorComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	TmpDoorComp->SetWorldLocation(Transform.GetCenter());

	const auto Rot = FQuat::MakeFromEuler(FVector(0, 0, Transform.Rot));
	TmpDoorComp->SetWorldRotation(Rot);
	
	TmpDoorComp->SetVisibility(true);

	TmpDoorComp->Mobility = EComponentMobility::Stationary;
	// TmpDoorComp->Mobility = EComponentMobility::Movable;
	TmpDoorComp->bUseDefaultCollision = false;
	TmpDoorComp->SetStaticMesh(DoorMesh);
	
	Doors.Add(TmpDoorComp);
}

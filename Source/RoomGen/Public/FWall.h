// Nikita Sergeev

#pragma once

#include "CoreMinimal.h"
#include "FWall.generated.h"

USTRUCT()
struct FKube
{
	GENERATED_BODY()

	UPROPERTY()
	float X = 0;
	UPROPERTY()
	float Y = 0;
	UPROPERTY()
	float Z = 30;

	UPROPERTY()
	float XOff = 0;
	UPROPERTY()
	float YOff = 0;
	UPROPERTY()
	float ZOff = 0;
};

USTRUCT()
struct FWall
{
	GENERATED_BODY()

	UPROPERTY()
	FKube Transform;

	UPROPERTY()
	TArray<FKube> Subtract;
};
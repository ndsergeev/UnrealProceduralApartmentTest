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

	inline bool IsValid() const
	{
		return X < XOff && Y < YOff && Z < ZOff;
	}
	
	void TrySwap()
	{
		if (X > XOff && Y > YOff && Z > ZOff)
		{
			Swap(X, XOff);
			Swap(Y, YOff);
			Swap(Z, ZOff);
		}
	}
	
private:
	template<class T>
	inline void Swap(T A, T B)
	{
		T Tmp = A;
		A = B;
		B = Tmp;
	}
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
// Nikita Sergeev

#pragma once

#include "FWall.h"
#include "CoreMinimal.h"

/**
 * 
 */
class ROOMGEN_API JSONParser
{
private:
	FString Path = "";
	
public:
	JSONParser(FString Path);

	bool Parse(TArray<FWall> &Walls) const;
	static void SetTransform(FKube &Transform, const TSharedPtr<FJsonObject> &Obj);
};

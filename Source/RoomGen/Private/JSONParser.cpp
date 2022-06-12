// Nikita Sergeev

#include "JSONParser.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

JSONParser::JSONParser(FString Path)
{
    this->Path = Path;
}

bool JSONParser::Parse(TArray<FWall> &Walls) const
{
    if (!FPaths::FileExists(Path))
        return false;

    FString JsonStr;
    FFileHelper::LoadFileToString(JsonStr, *Path);
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);
    
    TArray<TSharedPtr<FJsonValue>> JsonObject;
    if (!FJsonSerializer::Deserialize(Reader,JsonObject))
        return false;
    
    for (int32 w = 0; w < JsonObject.Num(); ++w)
    {
        FWall Wall;
        TSharedPtr<FJsonObject> WallObj = JsonObject[w]->AsObject();
        SetTransform(Wall.Transform, WallObj);

        const TArray<TSharedPtr<FJsonValue>> SubtractArr = WallObj->GetArrayField("Subtract");
        for (int32 s = 0; s < SubtractArr.Num(); ++s)
        {
            FKube Subtract;
            TSharedPtr<FJsonObject> SubtractObj = SubtractArr[s]->AsObject();
            SetTransform(Subtract, SubtractObj);
            
            Wall.Subtract.Add(Subtract);
        }
        
        Walls.Add(Wall);
    }
    
    return true;
}

inline void JSONParser::SetTransform(FKube &Transform, const TSharedPtr<FJsonObject> &Obj)
{
    const TSharedPtr<FJsonObject> SubtractTransformObj = Obj->GetObjectField("Transform");
    Transform.X    = SubtractTransformObj->GetNumberField("X");
    Transform.Y    = SubtractTransformObj->GetNumberField("Y");
    Transform.Z    = SubtractTransformObj->GetNumberField("Z");
    
    Transform.XOff = SubtractTransformObj->GetNumberField("XOff");
    Transform.YOff = SubtractTransformObj->GetNumberField("YOff");
    Transform.ZOff = SubtractTransformObj->GetNumberField("ZOff");
}
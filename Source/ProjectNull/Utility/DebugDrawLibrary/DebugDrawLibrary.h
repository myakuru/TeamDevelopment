#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DebugDrawLibrary.generated.h"


UCLASS()
class PROJECTNULL_API UDebugDrawLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// 扇状をデバッグライン可視化
	/// </summary>
	/// <param name="World">ワールド</param>
	/// <param name="Center">中心座標</param>
	/// <param name="Forward">前方方向</param>
	/// <param name="Radius">半径</param>
	/// <param name="AngleDeg">角度</param>
	/// <param name="Segments">区間</param>
	static void DrawDebugFan(UWorld* World,FVector Center,FVector Forward,float Radius,float AngleDeg,int32 Segments);
	

};

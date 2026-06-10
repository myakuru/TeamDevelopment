#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// ------------------------------------------------------------------------------------
// ID型で識別されしものが実装すべきInterface
// ------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FFooObjectId 
{
	GENERATED_BODY()
};

// This class does not need to be modified.
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class PROJECTNULL_API UFooObjectIdProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTNULL_API IFooObjectIdProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	// 純粋仮想関数で実装
	// インターフェースを使いたいものはこれをオーバーライドしたものを持つ
	virtual FFooObjectId GetFooObjectId() const = 0;

};

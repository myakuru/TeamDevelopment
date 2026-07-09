#include "WarningShapeActor.h"

// Sets default values
AWarningShapeActor::AWarningShapeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWarningShapeActor::BeginPlay()
{
	Super::BeginPlay();
	
	/**
	 * 生成から消滅までの流れ備忘録
	 * ①形状攻撃クラスがこのクラス(派生先を所持)
	 * ②攻撃が発動された時に警告を出すかのフラグで判断
	 * ③もし、発動するなら
	 */
	
	/**
	 * クラスの処理備忘録
	 */
}

// Called every frame
void AWarningShapeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


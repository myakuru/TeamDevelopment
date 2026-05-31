
#include "LaserBullet.h"

#include "GameFramework/ProjectileMovementComponent.h"

ALaserBullet::ALaserBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// ================================================================
	// ルートコンポーネントの初期化
	// ================================================================
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// ================================================================
	// スケルタルメッシュの初期化
	// ================================================================
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (!StaticMesh) { return; }
	StaticMesh->SetupAttachment(Root);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ================================================================
	// 飛び道具移動専用コンポーネントの初期化
	// ================================================================
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if (!ProjectileMovement) { return; }
	ProjectileMovement->UpdatedComponent = Root;
}

void ALaserBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaserBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


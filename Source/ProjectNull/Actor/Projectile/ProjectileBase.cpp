
#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>



AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;
	// ================================================================
	// ルートコンポーネントの初期化
	// ================================================================
	/*Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);*/

	// ================================================================
	// スフィアコリジョンコンポーネントの初期化
	// ================================================================
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//if (!SphereCollision) { return; }
	//
	SetRootComponent(SphereCollision);

	//SphereCollision->SetupAttachment(Root);

	SphereCollision->SetCollisionEnabled(
		ECollisionEnabled::QueryAndPhysics);

	SphereCollision->SetGenerateOverlapEvents(true);

	SphereCollision->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1,
		ECR_Overlap);

	SphereCollision->SetCollisionResponseToChannel(
		ECC_WorldStatic,
		ECR_Block);

	SphereCollision->SetCollisionResponseToChannel(
		ECC_WorldDynamic,
		ECR_Block);

	// ================================================================
	// スケルタルメッシュの初期化
	// ================================================================
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (!StaticMesh) { return; }
	StaticMesh->SetupAttachment(SphereCollision);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ================================================================
	// 飛び道具移動専用コンポーネントの初期化
	// ================================================================
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if (!ProjectileMovement) { return; }
	ProjectileMovement->UpdatedComponent = SphereCollision;
}

void AProjectileBase::BeginPlay()
{

	Super::BeginPlay();

	if (!SphereCollision) { return; }

	SphereCollision->OnComponentBeginOverlap.AddDynamic(
		this,
		&AProjectileBase::OnCollisionOverlap);

	if (!ProjectileMovement) { return; }

	ProjectileMovement->OnProjectileStop.AddDynamic(
		this,
		&AProjectileBase::OnProjectileStop);

	//UE_LOG(LogTemp, Display, TEXT("呼!"));

}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::OnCollisionOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("当たった!"));
	if (!OtherActor || OtherActor == this || !OwnerActor) { return; }

	auto Enemy = Cast<AEnemyBase>(OtherActor);

	const FVector Location = OwnerActor->GetActorLocation();

	if (!Enemy) { return; }
	Enemy->SetKnockBackData(Location, 2.0f, 1.0f);

	// ダメージを与える
	Enemy->SetTakeDamaged();

	Destroy();
}

void AProjectileBase::OnProjectileStop(const FHitResult& Hit)
{
	Destroy();

}


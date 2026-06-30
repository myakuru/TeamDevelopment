
#include "ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Effect/EffectBase.h>


AProjectileBase::AProjectileBase():
	Root(nullptr),
	SphereCollision(nullptr),
	StaticMesh(nullptr),
	ProjectileMovement(nullptr),
	AliveTimerHandle(FTimerHandle()),
	AliveTime(2.f)
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// ================================================================
	// スフィアコリジョンコンポーネントの初期化
	// ================================================================
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	if (!SphereCollision) { return; }

	SphereCollision->SetupAttachment(Root);

	SphereCollision->SetCollisionEnabled(
		ECollisionEnabled::QueryAndPhysics);

	SphereCollision->SetGenerateOverlapEvents(true);

	SphereCollision->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1,
		ECR_Overlap);

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

	GetWorld()->GetTimerManager().SetTimer(
		AliveTimerHandle,
		this,
		&AProjectileBase::ActorDestroy,
		AliveTime,
		false);

	if (!ProjectileEffect) { return; }
	ProjectileEffect->Start(Root);
}

void AProjectileBase::HandleCollision(AActor* OtherActor)
{
	if (!OtherActor || 
		OtherActor == this ||
		!OwnerActor) { return; }

	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	{
		Interface->ApplyDamaged();
		Interface->ApplyKnockBack(OwnerActor->GetActorLocation());
	}

	Destroy();
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
	HandleCollision(OtherActor);
}

void AProjectileBase::OnProjectileStop(const FHitResult& Hit)
{
	Destroy();

}

void AProjectileBase::ActorDestroy()
{
	Destroy();

}


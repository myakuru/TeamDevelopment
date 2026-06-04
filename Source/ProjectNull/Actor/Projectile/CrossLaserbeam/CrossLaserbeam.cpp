
#include "CrossLaserbeam.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>


ACrossLaserbeam::ACrossLaserbeam()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	LaserBoxes.SetNum(4);

	for (int32 Index = 0; Index < 4; ++Index)
	{
		auto& Box = LaserBoxes[Index];

		FString Name = FString::Printf(
			TEXT("LaserBox_%d"),
			Index);

		Box = CreateDefaultSubobject<UBoxComponent>(*Name);
		if (!Box) { continue; }
		Box->SetupAttachment(RootComponent);
		Box->SetCollisionEnabled(
			ECollisionEnabled::QueryOnly);

		Box->SetGenerateOverlapEvents(true);
		Box->SetCollisionResponseToChannel(
			ECC_GameTraceChannel1,
			ECR_Overlap);
		Box->OnComponentBeginOverlap.AddDynamic(
			this,
			&ACrossLaserbeam::OnLaserBeginOverlap);

		Box->OnComponentEndOverlap.AddDynamic(
			this,
			&ACrossLaserbeam::OnLaserEndOverlap);
	}
}

void ACrossLaserbeam::BeginPlay()
{
	Super::BeginPlay();
	
	/*GetWorld()->GetTimerManager().SetTimer(
		HitIntervalTimerHandle,
		this,
		&ACrossLaserbeam::OnHit,
		HitInterval,
		true);*/
}

void ACrossLaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrossLaserbeam::SetLaserEnabled(bool bEnabled)
{
	const ECollisionEnabled::Type CollisionType =
		bEnabled
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision;

	for (auto& Box : LaserBoxes)
	{
		if (!Box) { continue; }

		Box->SetCollisionEnabled(CollisionType);
	}
}

void ACrossLaserbeam::OnLaserBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) { return; }

	auto Enemy = Cast<AEnemyBase>(OtherActor);
	if (!Enemy) { return; }
	OnHit();
	//HitEnemies.Add(Enemy);
}

void ACrossLaserbeam::OnLaserEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	auto Enemy = Cast<AEnemyBase>(OtherActor);

	if (!Enemy) { return; }

	//HitEnemies.Remove(Enemy);
}

void ACrossLaserbeam::OnHit()
{
	for (auto& HitEnemy : HitEnemies) 
	{
		auto Enemy = HitEnemy.Get();
		if (!Enemy) { continue; }
		Enemy->SetKnockBackData(GetActorLocation(), 2.0f, 1.0f);
		Enemy->SetTakeDamaged();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearSkill.h"

#include "../Character/CombatCharacterBase/Enemy/EnemyBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../../Utility/Common/Definitions/CollisionChannels.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AExplosionGearSkill::AExplosionGearSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("AExplosionGearSkill Constructer"));
	PrimaryActorTick.bCanEverTick = false;

	// SphereComponent追加(引数はエディタで表示する名前)
	Collision = CreateDefaultSubobject<USphereComponent>(
		TEXT("Collision")
	);

	// 自身をPlayerAttackOverlapとして設定
	Collision->SetCollisionObjectType(
		PLAYER_ATTACK_OVERLAP
	);

	// 当たり判定の種類設定(物理衝突を行わず、判定のみ行う)
	Collision->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly
	);

	// 当たり判定を行う相手を無しで初期化
	Collision->SetCollisionResponseToAllChannels(
		ECR_Ignore
	);

	// Pawnチャンネル(キャラ系)に対して重なり判定を設定
	Collision->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1,
		ECR_Overlap
	);

	//このActorの基準となるコンポーネントを当たり判定コンポーネントに設定
	RootComponent = Collision;

}

void AExplosionGearSkill::BeginPlay()
{
	Super::BeginPlay();

	// 生成されたDelay秒後に爆発
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&AExplosionGearSkill::Explode,
		Data.Delay,
		false
	);
	
}

//SpawnActorDeferredを使ってBeginPlay前にInitializeを呼ぶ必要あり
void AExplosionGearSkill::Initialize(const FExplosionData& InData)
{
	Data = InData;

	Collision->SetSphereRadius(Data.Radius);
	Collision->SetGenerateOverlapEvents(true);
}

void AExplosionGearSkill::Explode()
{
	if (ExplosionFX) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionFX,
			GetActorLocation()
		);
	}

	// OverlapしているActorを検索
	TArray<AActor*> actors; 
	Collision->UpdateOverlaps();
	Collision->GetOverlappingActors(actors);
	UE_LOG(LogTemp, Warning, TEXT("Radius = %f"),Data.Radius);
	UE_LOG(LogTemp, Warning,TEXT("HitActorNum = %d"),actors.Num());

	// 検索したActorからEnemyBaseを見つけてヒット処理を行う
	for (AActor* actor : actors) {
		AEnemyBase* enemy = Cast<AEnemyBase>(actor);
		if (!enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Not Enemy"));
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("TakeDamege"));
		enemy->SetKnockBackData(GetActorLocation(), 4, 1);
		enemy->SetTakeDamaged(Data.Damage);
	}


	Destroy();
}


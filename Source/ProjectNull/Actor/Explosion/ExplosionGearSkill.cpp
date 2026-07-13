// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearSkill.h"

#include "../Character/CombatCharacterBase/Enemy/EnemyBase.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../../Utility/Common/Definitions/CollisionChannels.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"

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
		ECC_PlayerAttack
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

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this] {
			if (PreExplosionFX) {
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					PreExplosionFX,
					GetActorLocation()
				);
			}
		}
	);

	// 生成されたDelay秒後に発火エフェクト生成
	GetWorldTimerManager().SetTimer(
		PreExplosionTimerHandle,
		timerDelegate,
		Data.Delay,
		false
	);

	// 生成されたIgnitionDelay + Delay秒後に爆発
	GetWorldTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AExplosionGearSkill::Explode,
		Data.IgnitionDelay + Data.Delay,
		false
	);
	
}

//SpawnActorDeferredを使ってBeginPlay前にInitializeを呼ぶ必要あり
void AExplosionGearSkill::Initialize(const FExplosionData& InData)
{
	Data = InData;

	ApplyData();
}

void AExplosionGearSkill::ApplyData()
{
	Collision->SetSphereRadius(CollisionRadius * Data.Scale);
	Collision->SetGenerateOverlapEvents(true);
}

void AExplosionGearSkill::Explode()
{
	
	// 爆発エフェクト再生
	if (ExplosionFX) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionFX,
			GetActorLocation()
		);

		NiagaraComp->SetWorldScale3D(FVector(Data.Scale));

	}

	// カメラシェイクを爆発のスケールに応じて再生
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	if (playerController) {
		playerController->ClientStartCameraShake(
			ExplosionCameraShakeClass,
			Data.Scale
		);
	}

	// OverlapしているActorを検索
	TArray<AActor*> actors; 
	Collision->UpdateOverlaps();
	Collision->GetOverlappingActors(actors);

	// 検索したActorからEnemyBaseを見つけてヒット処理を行う
	for (AActor* actor : actors) {
		AEnemyBase* enemy = Cast<AEnemyBase>(actor);
		if (!enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Not Enemy"));
			continue;
		}

		// キャラクターインターフェースを実装しているか
		if (auto* interface = Cast<ICharacterInterface>(enemy))
		{
			interface->ApplyDamaged(Data.Damage);
			interface->ApplyKnockBack(GetActorLocation());
		}
	}


	Destroy();
}


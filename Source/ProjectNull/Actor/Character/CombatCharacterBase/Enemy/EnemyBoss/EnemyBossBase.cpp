
#include "EnemyBossBase.h"
#include "AIC_EnemyBoss.h"
#include "Components/StateTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/DamageNumberPoolSubsystem/DamageNumberPoolSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Stage/Manager/StageManager.h>

// Sets default values
AEnemyBossBase::AEnemyBossBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);		// 片側６０°（視界１２０°）
	PawnSensingComp->SightRadius = 2000.0f;					// 視認距離
	// PawnSensingComponentはデフォルトでbOnlySensePlayers = trueなので
	// OnSeePawnはプレイヤーが視界・距離・視線条件を満たしたときだけ発火する

	// 敵のランタイムパラメータ管理クラスの生成
	EnemyBossRuntimeData = CreateDefaultSubobject<UEnemyBossRuntimeData>("EnemyBossRuntimeData");

	StateTreeComp = CreateDefaultSubobject<UStateTreeComponent>("StateTreeComponent");

	BreathEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BreathNiagara"));
	BreathEffect->SetupAttachment(GetMesh(), FName("tongue_05"));
	BreathEffect->SetAutoActivate(false);
	BreathEffect->SetRelativeRotation(NiagaraRotOffset);
}

// Called when the game starts or when spawned
void AEnemyBossBase::BeginPlay()
{
	Super::BeginPlay();

	//// 視界トリガー：プレイヤーが視界に入ったら追尾対象に設定
	//if (IsValid(PawnSensingComp))
	//{
	//	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBossBase::OnSeePlayer);
	//}

	// 攻撃トリガー：ダメージを受けたら攻撃者を追尾対象に設定
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBossBase::HandleTakeAnyDamage);

	RegisterDelegates();

	USkeletalMeshComponent* MeshComp = GetMesh();
	if (MeshComp)
	{
		MeshComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (Capsule)
	{
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	// ボスに設定されているマテリアルをC++から値を変更できるDynamicMaterialに変換して保存
	const int32 MaterialNum = GetMesh()->GetNumMaterials();

	DynamicMaterials.Empty();

	for (int32 i = 0; i < MaterialNum; ++i)
	{
		// i番目のマテリアルをDynamicMaterialInstanceに変換してメッシュにセット
		UMaterialInstanceDynamic* MID = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
		if (IsValid(MID))
		{
			// 被弾したときにすべてのマテリアルをfor文で回せるようにするためリストに保存
			DynamicMaterials.Add(MID);

			// マテリアルで使うデータ
			MID->SetScalarParameterValue(TEXT("HitPower"), 0.0f);
			MID->SetScalarParameterValue(TEXT("HitRadius"), 80.0f);
			MID->SetVectorParameterValue(TEXT("HitColor"), FLinearColor::Red);
			MID->SetScalarParameterValue(TEXT("NoiseScale"), 2.0f);
			MID->SetScalarParameterValue(TEXT("NoisePower"), 1.5f);
			MID->SetScalarParameterValue(TEXT("HitEmissivePower"), 3.0f);
		}
	}

	SetEnemyBossStatusData(EnemyDataAsset);

	// ヒットポイントの更新
	{
		// 計算後の最終的なHPをセット
		EnemyBossRuntimeData->SetFinalHP(EnemyBossStatus.FinalHP);
	}

	// 攻撃パラメータの更新
	{
		// 最終的な攻撃倍率の更新
		float AttackScale = EnemyBossStatus.FinalAttack;
	}

	// 座標
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!IsValid(PlayerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawnが取得できません"));
		return;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FVector SpawnLocation = CalculateEnemySpawnPointInRing(PlayerLocation);

	FHitResult HitResult;
	const bool bHitGround = IsIntersectingStaticObjects(HitResult, SpawnLocation);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Player=%s Candidate=%s Hit=%s HitActor=%s"),
		*PlayerLocation.ToString(),
		*SpawnLocation.ToString(),
		bHitGround ? TEXT("true") : TEXT("false"),
		*GetNameSafe(HitResult.GetActor())
	);

	if (!bHitGround)
	{
		UE_LOG(LogTemp, Error, TEXT("地面を検出できなかったため、ボスが初期位置に残ります"));
		return;
	}
	SetActorLocation(SpawnLocation);

	//SpawnedDeathEffect =
	//	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	//		GetWorld(),
	//		DeathEffect,
	//		GetActorLocation(),
	//		GetActorRotation(),
	//		GetActorScale3D(),
	//		false, // 後から止めるためAutoDestroyはfalse
	//		true,
	//		ENCPoolMethod::None,
	//		false
	//	);

}

// Called every frame
void AEnemyBossBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::J))
	{
		EnemyBossRuntimeData->AddHealthDebug();
	}

	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (IsValid(Anim))
	{
		UAnimMontage* CurrentMontage = Anim->GetCurrentActiveMontage();
		if (IsValid(CurrentMontage))
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentMontage = %s"), *CurrentMontage->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CurrentMontage = None"));
		}
	}

	if (DeathMaterialChangeFlg)
	{
		BossDeathMaterialChange();
	}

	APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PPlayerPawn) { return; }

	SetTargetActor(PPlayerPawn);
}

// ------------------------------------------------------------------------------------
// 被弾処理
// ------------------------------------------------------------------------------------
//void AEnemyBossBase::ReceiveDamage(float Damage)
//{
//	UE_LOG(LogTemp, Warning, TEXT("Boss Receive Damage "));
//}

void AEnemyBossBase::ApplyDamaged(float InDamaged)
{
	if (!EnemyBossRuntimeData) { return; }

	EnemyBossRuntimeData->AddHealth(-InDamaged);		// 渡された値分、FinalHPを減算
	UE_LOG(LogTemp, Error, TEXT("Damage : %f"), InDamaged);

	EnemyBossRuntimeData
		->CalclateDamageToMaxHealthRatio(InDamaged);// 受けたダメージが最大体力に対して何割かを算出
	OnHit();

	// ダメージUI表示位置
	const FVector DamageUILocation = GetActorLocation() + FVector(0.0f, 0.0f, 120.0f);

	if (UDamageNumberPoolSubsystem* Pool = GetWorld()->GetSubsystem<UDamageNumberPoolSubsystem>())
	{
		Pool->ShowDamageNumber(DamageUILocation, InDamaged, false);
	}

	// 体力が0以下なら死亡フラグを立てる
	if (EnemyBossRuntimeData->GetHealth() <= 0)
	{
		EnemyBossStatus.IsAlive = false;
		EnemyBossRuntimeData->ChangedIsAlive(EnemyBossStatus.IsAlive);
		OnDeath();
	}
}

void AEnemyBossBase::OnHit()
{

}

void AEnemyBossBase::RegisterDelegates()
{
	// デリゲートへの登録はBeginPlayで行う
}

void AEnemyBossBase::ResetGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
}

void AEnemyBossBase::TryConsumeFastFallRequest()
{
	if (!EnemyBossRuntimeData->bShouldFastFallOnNotify) return;

	EnemyBossRuntimeData->bShouldFastFallOnNotify = false;

	GetCharacterMovement()->GravityScale = FastFallGravityScale;

}

// ------------------------------------------------------------------------------------
// 視界確認処理
// ------------------------------------------------------------------------------------
void AEnemyBossBase::OnSeePlayer(APawn* Pawn)
{
	if (!IsValid(Pawn)) { return; }

	SetTargetActor(Pawn);

	// 視野に入ったら画面に"See"と表示
	//UKismetSystemLibrary::PrintString(this, "See", true, true, FColor::Blue, 2.0f);
}

// ------------------------------------------------------------------------------------
// 被弾時にターゲットを確定する処理
// ------------------------------------------------------------------------------------
void AEnemyBossBase::HandleTakeAnyDamage(AActor* DamagedActor, float Damage,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// 攻撃者を特定。コントローラ経由のPawnを優先し、なければDamageCauserを使う。
	AActor* NewTarget = nullptr;
	if (IsValid(InstigatedBy))
	{
		NewTarget = InstigatedBy->GetPawn();
	}
	if (!IsValid(NewTarget))
	{
		NewTarget = DamageCauser;
	}

	if (IsValid(NewTarget))
	{
		SetTargetActor(NewTarget);
	}
}

void AEnemyBossBase::SetEnemyBossStatusData(UEnemyBossDataAsset* InData)
{
	if (!InData) { return; }

	EnemyBossStatus.MoveSpeed = InData->MoveSpeed;
	EnemyBossStatus.RotationInterpSpeed = InData->RotationInterpSpeed;
	EnemyBossStatus.FinalHP = InData->FinalHP;
	EnemyBossStatus.FinalAttack = InData->FinalAttack;
	EnemyBossStatus.KnockBackWeight = InData->KnockBackWeight;
	EnemyBossStatus.Exp = InData->Exp;
	EnemyBossStatus.GearEnergy = InData->GearEnergy;
}

void AEnemyBossBase::ApplyLocalHitPos(const FVector& HitWorldLocation)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!IsValid(MeshComp))
	{
		return;
	}

	// ワールド座標のヒット位置を、ボスメッシュのローカル座標へ変換
	const FVector HitLocalPos = MeshComp->GetComponentTransform().InverseTransformPosition(HitWorldLocation);

	for (UMaterialInstanceDynamic* MID : DynamicMaterials)
	{
		if (!IsValid(MID))
		{
			continue;
		}

		MID->SetVectorParameterValue(
			TEXT("HitLocalPos"),
			FLinearColor(HitLocalPos.X, HitLocalPos.Y, HitLocalPos.Z, 1.0f)
		);

		MID->SetScalarParameterValue(TEXT("HitRadius"), HitRadius);
		MID->SetScalarParameterValue(TEXT("HitPower"), HitPower);
		MID->SetVectorParameterValue(TEXT("HitColor"), FLinearColor(HitColor.X, HitColor.Y, HitColor.Z, 1.0f));
		MID->SetScalarParameterValue(TEXT("NoiseScale"), NoiseScale);
		MID->SetScalarParameterValue(TEXT("NoisePower"), NoisePower);
		MID->SetScalarParameterValue(TEXT("HitEmissivePower"), HitEmissivePower);
	}

	GetWorldTimerManager().ClearTimer(HitFlashTimerHandle);
	GetWorldTimerManager().SetTimer(
		HitFlashTimerHandle,
		this,
		&AEnemyBossBase::EndLocalHitFlash,
		HitTimeDuration,	// 継続時間
		false);
}

void AEnemyBossBase::EndLocalHitFlash()
{
	for (UMaterialInstanceDynamic* MID : DynamicMaterials)
	{
		if (!IsValid(MID))
		{
			continue;
		}
		MID->SetScalarParameterValue(TEXT("HitPower"), 0.0f);
	}
}

void AEnemyBossBase::BossFinalize()
{
	if (IsValid(SpawnedDeathEffect))
	{
		SpawnedDeathEffect->DeactivateImmediate();
		SpawnedDeathEffect->SetVisibility(false, true);
		SpawnedDeathEffect->DestroyComponent();
		SpawnedDeathEffect = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("DeathEffectを停止しました"));
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	//SpawnDeathEffect();

	if (DestroyEffect)
	{
		FTransform AdjustedTransform = GetActorTransform();
		FRotator Rot = AdjustedTransform.GetRotation().Rotator();
		Rot.Yaw -= 90.0f;
		AdjustedTransform.SetRotation(Rot.Quaternion());

		FVector Loc = AdjustedTransform.GetLocation();
		Loc.Z -= 90.0f;
		AdjustedTransform.SetLocation(Loc);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DestroyEffect,
			AdjustedTransform.GetLocation(),
			AdjustedTransform.Rotator(),
			FVector(1.0f),
			true,   // bAutoDestroy
			true,   // bAutoActivate
			ENCPoolMethod::None,
			true    // bPreCullCheck
		);
	}

	//DestroyEffect->ReleaseRef();

	// StateTreeを停止
	if (StateTreeComp)
	{
		StateTreeComp->StopLogic(TEXT("Deactivate"));
	}

	/*if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		GameInstance->GetStageManagerSubsystem()->InGameFinalize();
	}*/

	if (UEnemyManagerSubsystem* EnemyManager =
		GetWorld()->GetSubsystem<UEnemyManagerSubsystem>())
	{
		EnemyManager->DestroyAllEnemy();
	}

	if (auto* GameProgress = GetWorld()->GetSubsystem<UGameProgressSubsystem>())
	{
		GameProgress->AddKillBossCount();
	}
}

void AEnemyBossBase::BossDeathMaterialChange()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!IsValid(MeshComp))
	{
		return;
	}

	// 変化させるマテリアルの中心を決める（ソケットから）
	//const FVector StartLocalPos = MeshComp->GetSocketTransform(MaterialNodeName).GetTranslation();

	FVector StartLocalPos =
		MeshComp->GetSocketTransform(
			MaterialNodeName,
			RTS_Component
		).GetLocation();

	StartLocalPos = GetActorLocation();

	for (UMaterialInstanceDynamic* MID : DynamicMaterials)
	{
		if (!IsValid(MID))
		{
			continue;
		}

		MID->SetVectorParameterValue(
			TEXT("HitLocalPos"),
			FLinearColor(StartLocalPos.X, StartLocalPos.Y, StartLocalPos.Z, 1.0f)
		);

		//HitRadius += RadiusOffset;
		//HitEmissivePower += HitEmissivePowerOffset;

		MID->SetScalarParameterValue(TEXT("HitRadius"), HitRadius);
		MID->SetScalarParameterValue(TEXT("HitPower"), HitPower);
		MID->SetVectorParameterValue(TEXT("HitColor"), FLinearColor(HitColor.X, HitColor.Y, HitColor.Z, 1.0f));
		MID->SetScalarParameterValue(TEXT("NoiseScale"), NoiseScale);
		MID->SetScalarParameterValue(TEXT("NoisePower"), NoisePower);
		MID->SetScalarParameterValue(TEXT("HitEmissivePower"), HitEmissivePower);
	}
}

void AEnemyBossBase::SpawnDeathEffect()
{
	if (IsValid(SpawnedDeathEffect))
	{
		SpawnedDeathEffect->DeactivateImmediate();
		SpawnedDeathEffect->DestroyComponent();
		SpawnedDeathEffect = nullptr;
	}

	// 敵が死んだ際にパーティクルを出す
	if (DeathEffect)
	{
		FTransform AdjustedTransform = GetActorTransform();
		FRotator Rot = AdjustedTransform.GetRotation().Rotator();
		Rot.Yaw -= 90.0f;
		AdjustedTransform.SetRotation(Rot.Quaternion());

		FVector Loc = AdjustedTransform.GetLocation();
		Loc.Z -= 90.0f;
		AdjustedTransform.SetLocation(Loc);

		FTransform MeshTransform = GetMesh()->GetComponentTransform();

		SpawnedDeathEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			MeshTransform.GetLocation(),
			MeshTransform.Rotator(),
			MeshTransform.GetScale3D(),
			false,		// bAutoDestroy
			true,		// bAutoActivate
			ENCPoolMethod::None,
			true		// bPreCullCheck
		);
	}
}



bool AEnemyBossBase::IsIntersectingStaticObjects(FHitResult& HitResult, FVector& SpawnLocationXY)
{
	// Rayの座標を求める
	FVector RayStart = SpawnLocationXY + FVector(0.0f, 0.0f, SpawnRayStartHeight);
	FVector RayEnd = SpawnLocationXY - FVector(0.0f, 0.0f, SpawnRayEndDepth);

	// Rayがワールドの静的オブジェクトに衝突しているか調べる
	const bool IsIntersect = GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Visibility);

	// 衝突していたら衝突した座標を出現座標にする
	if (IsIntersect)
	{
		SpawnLocationXY = HitResult.Location;
	}

	return IsIntersect;
}

FVector AEnemyBossBase::CalculateEnemySpawnPointInRing(const FVector& Center) const
{
	// ランダム角度
	float angle = FMath::RandRange(0.0f, 360.0f);

	// XYオフセット
	FVector offset = {
		FMath::Cos(FMath::DegreesToRadians(angle)) * SpawnRadius,
		FMath::Sin(FMath::DegreesToRadians(angle)) * SpawnRadius,
		0.0f
	};

	return Center + offset;
}
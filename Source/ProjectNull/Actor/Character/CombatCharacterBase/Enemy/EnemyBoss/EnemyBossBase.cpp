
#include "EnemyBossBase.h"
#include "AIC_EnemyBoss.h"
#include "Components/StateTreeComponent.h"
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>
#include "Perception/PawnSensingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

// Called when the game starts or when spawned
void AEnemyBossBase::BeginPlay()
{
	Super::BeginPlay();

	// 視界トリガー：プレイヤーが視界に入ったら追尾対象に設定
	if (IsValid(PawnSensingComp))
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBossBase::OnSeePlayer);
	}

	// 攻撃トリガー：ダメージを受けたら攻撃者を追尾対象に設定
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBossBase::HandleTakeAnyDamage);

	RegisterDelegates();

	//SetBossEnemyStatus();

}

// Called every frame
void AEnemyBossBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);
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
}

// ------------------------------------------------------------------------------------
// 被弾処理
// ------------------------------------------------------------------------------------
void AEnemyBossBase::ReceiveDamage(float Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Receive Damage "));
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

	EnemyBossStatus.MoveSpeed			= InData->MoveSpeed;
	EnemyBossStatus.RotationInterpSpeed = InData->RotationInterpSpeed;
	EnemyBossStatus.FinalHP				= InData->FinalHP;
	EnemyBossStatus.FinalAttack			= InData->FinalAttack;
	EnemyBossStatus.KnockBackWeight		= InData->KnockBackWeight;
	EnemyBossStatus.Exp					= InData->Exp;
	EnemyBossStatus.GearEnergy			= InData->GearEnergy;
}

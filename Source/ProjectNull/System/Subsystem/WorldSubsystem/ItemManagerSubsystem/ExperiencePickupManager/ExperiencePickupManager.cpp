#include "ExperiencePickupManager.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "GameFramework/Pawn.h"

/**
* Niagaraシステム側で用意するUserParameterの名前
*/
namespace ExperienceNiagaraParams
{
	static const FName Positions = TEXT("User.Positions");
	static const FName Colors = TEXT("User.Colors");
	static const FName Sizes = TEXT("User.Sizes");
}

// 
// 初期化：Actor生成からNiagara起動までを行う
// 
void FExperiencePickupManager::Initialize(UWorld* World)
{
	if (!World) { return; }

	UNiagaraSystem* LoadedNiagaraSystem = LoadObject<UNiagaraSystem>(
		nullptr,
		TEXT("/Game/FreeNiagaraPack/Effects/Matsuura_Test_Niagara/ExperiencePickup.ExperiencePickup")
		// ↑ アセットを右クリック → Copy Reference で正確なパスを取得
	);

	if (!LoadedNiagaraSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: NiagaraSystemのロードに失敗しました"));
	}


	// 描画専用Actorをスポーン
	AActor* Owner = World->SpawnActor<AActor>();
	if (!Owner) { return; }

	NiagaraOwnerActor = Owner;

	// NiagaraComponentを生成・アタッチ
	UNiagaraComponent* NiagaraComp = NewObject<UNiagaraComponent>(Owner);
	NiagaraComp->RegisterComponent();
	NiagaraComp->AttachToComponent(
		Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	if (LoadedNiagaraSystem)
	{
		NiagaraComp->SetAsset(LoadedNiagaraSystem);
		NiagaraComp->Activate();
	}
	NiagaraComponent = NiagaraComp;
}

void FExperiencePickupManager::Update(APawn* Player, float DeltaTime)
{
	if (!Player) { return; }

	const FVector PlayerLocation = Player->GetActorLocation();

	UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: Update"));

	// 逆順ループで安全にRemoveを行う
	for (int32 i = ExperienceList.Num() - 1; i >= 0; --i)
	{
		FExperiencePickupData& Exp = ExperienceList[i];

		// 寿命チェック
		Exp.LifeTime -= DeltaTime;
		if (Exp.LifeTime <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: TimeRemove"));
			ExperienceList.RemoveAt(i);
			continue;
		}

		const float DistSq = FVector::DistSquared(Exp.Location, PlayerLocation);

		// 取得判定（PickupRange以内）
		if (DistSq <= FMath::Square(Exp.PickupRange))
		{
			UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: Remove"));
			PendingExpValue += Exp.ExpValue;
			ExperienceList.RemoveAt(i);
			continue;
		}

		// 吸引開始（DetectRange以内）
		if (!Exp.bChasing && DistSq <= FMath::Square(Exp.DetectRange))
		{
			UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: Chasing True"));
			Exp.bChasing = true;
		}

		// 吸引移動
		if (Exp.bChasing)
		{
			UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: Chasing Now"));
			const FVector Direction = (PlayerLocation - Exp.Location).GetSafeNormal();
			Exp.Location += Direction * Exp.ChaseSpeed * DeltaTime;
		}
	}

	// 毎フレーム最期にNiagaraへ同期
	SyncToNiagara();
}

void FExperiencePickupManager::SpawnExperience(
	const FVector&			Location,
	float							ExpValue,
	const FLinearColor&	Color,
	float							Size)
{
	FExperiencePickupData NewExp;
	NewExp.Location = Location;
	NewExp.ExpValue = ExpValue;
	NewExp.Color = Color;
	NewExp.Size = Size;

	ExperienceList.Add(NewExp);
}

void FExperiencePickupManager::Clear()
{
	ExperienceList.Empty();
	PendingExpValue = 0.0f;

	// NiagaraComponentを空配列で上書きしてから
	SyncToNiagara();

	// Actorを破棄（NiagaraComponentも連鎖して破棄される）
	if (NiagaraOwnerActor.IsValid())
	{
		NiagaraOwnerActor->Destroy();
		NiagaraOwnerActor = nullptr;
	}
}

float FExperiencePickupManager::ConsumeCollectedExp()
{
	const float Collected = PendingExpValue;
	PendingExpValue = 0.0f;
	return Collected;
}

void FExperiencePickupManager::SyncToNiagara()
{
	if (!NiagaraComponent.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("NiagaraComponent None"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("SyncToNiagara ParticleNum: %d"), ExperienceList.Num());
	
	if (!NiagaraComponent.IsValid()) { return; }

	const int32 Count = ExperienceList.Num();

	TArray<FVector>			Positions;
	TArray<FLinearColor>	Colors;
	TArray<float>				Sizes;

	Positions.Reserve(Count);
	Colors.Reserve(Count);
	Sizes.Reserve(Count);

	for (const FExperiencePickupData& Exp : ExperienceList)
	{
		Positions.Add(Exp.Location);
		Colors.Add(Exp.Color);
		Sizes.Add(Exp.Size);
	}

	// Niagaraの配列UserParameterへ書き込む
	// NiagaraシステムアセットにArray<Vector/LinearColor/Float>型の
	// UserParameterを同名で作成しておく必要がある
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(
		NiagaraComponent.Get(),
		ExperienceNiagaraParams::Positions,
		Positions);
 
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayColor(
		NiagaraComponent.Get(),
		ExperienceNiagaraParams::Colors,
		Colors);
 
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayFloat(
		NiagaraComponent.Get(),
		ExperienceNiagaraParams::Sizes,
		Sizes);
}

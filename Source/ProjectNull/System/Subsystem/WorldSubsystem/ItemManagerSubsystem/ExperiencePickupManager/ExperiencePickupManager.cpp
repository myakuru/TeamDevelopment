#include "ExperiencePickupManager.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "GameFramework/Pawn.h"

/**
* Niagaraシステム側で用意するUserParameterの名前
*/
namespace ExperienceNiagaraParams
{
	static const FName Positions	= TEXT("User.Positions");
	static const FName Colors		= TEXT("User.Colors");
	static const FName Sizes		= TEXT("User.Sizes");
	static const FName SpawnCount	= TEXT("User.SpawnCount");
}

// 
// 初期化：Actor生成からNiagara起動までを行う
// 
void FExperiencePickupManager::Initialize(UWorld* World)
{
	if (!World) { return; }

	UNiagaraSystem* LoadedNiagaraSystem = LoadObject<UNiagaraSystem>(
		nullptr,
		TEXT("/Game/Actor/Item/PickupItem/ExperiencePickup.ExperiencePickup")
		// アセットを右クリック → Copy Reference で正確なパスを取得
	);

	if (!LoadedNiagaraSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ExperiencePickupManager: NiagaraSystemのロードに失敗しました"));
	}


	// 描画専用Actorをスポーン
	//AActor* Owner = World->SpawnActor<AActor>();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Name = FName(TEXT("ExperienceNiagaraActor"));
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

	AActor* Owner = World->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	if (!Owner) { return; }

	//名前とラベルを変更
	Owner->SetIsTemporarilyHiddenInEditor(true);
#if WITH_EDITOR
	Owner->SetActorLabel(TEXT("ExperienceNiagaraActor"));
#endif

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
			PendingExpValue += Exp.ExpValue;
			ExperienceList.RemoveAt(i);
			continue;
		}

		// 吸引開始（DetectRange以内）
		if (!Exp.bChasing && DistSq <= FMath::Square(Exp.DetectRange))
		{
			Exp.bChasing = true;
			Exp.ChaseStartPos = Exp.Location;
			Exp.RandomBulgeWidth = FMath::FRandRange(-100.0f, 100.0f);
			Exp.RandomBulgeHeight = FMath::FRandRange(-100.0f, 100.0f);
		}

		// 吸引移動
		if (Exp.bChasing)
		{
			//const FVector Direction = (PlayerLocation - Exp.Location).GetSafeNormal();
			//Exp.Location += Direction * Exp.ChaseSpeed * DeltaTime;
			Exp.ChaseElapsedTime += DeltaTime;
			float t = FMath::Clamp(Exp.ChaseElapsedTime / DefaultSettings.ChaseDurationTime, 0.0f, 1.0f);
			Exp.Location = CalculateOrbit(
				Exp.ChaseStartPos,
				PlayerLocation,
				t,
				Exp.RandomBulgeWidth,
				Exp.RandomBulgeHeight);
		}
	}

	// 毎フレーム最期にNiagaraへ同期
	SyncToNiagara();
}

void FExperiencePickupManager::SpawnExperience(
	const FVector&					Location,
	float							ExpValue,
	const FLinearColor&	Color,
	float							Size)
{
	FExperiencePickupData NewExp;
	NewExp.Location = Location;
	NewExp.ExpValue = ExpValue;
	NewExp.Color	= Color;
	NewExp.Size		= Size;

	ExperienceList.Add(NewExp);
}

void FExperiencePickupManager::Clear()
{
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("-  FExperiencePickupManager:Clear  -"));

	ExperienceList.Empty();
	PendingExpValue = 0.0f;

	// NiagaraComponentを空配列で上書きしてから
	SyncToNiagara();

	// Actorを破棄（NiagaraComponentも連鎖して破棄される）
	if (NiagaraOwnerActor.IsValid())
	{
		NiagaraOwnerActor->Destroy();
		NiagaraOwnerActor = nullptr;

		UE_LOG(LogTemp, Log, TEXT("- NiagaraOwnerActor:Destroy -"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("- NiagaraOwnerActor:None -"));
	}

	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
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
	
	if (!NiagaraComponent.IsValid()) { return; }

	const int32 Count = ExperienceList.Num();

	// オーブが0個のときはNiagaraを止める
	// ナイアガラで、Spawn Burst Instantaneousを使用している場合
	// SpawnCountが０の時でも最低１個はパーティクルを発生してしまうため、非表示にしておく
	if (Count == 0)
	{
		if (NiagaraComponent->IsActive())
		{
			NiagaraComponent->DeactivateImmediate();
		}
		return;
	}

	// オーブが増えたとき非アクティブなら再起動
	if (!NiagaraComponent->IsActive())
	{
		NiagaraComponent->Activate();
	}

	TArray<FVector>			Positions;
	TArray<FLinearColor>	Colors;
	TArray<float>			Sizes;

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

	// 配列サイズをSpawnCountとして渡す
	NiagaraComponent->SetNiagaraVariableInt(
		TEXT("User.SpawnCount"), Count);
}

FVector FExperiencePickupManager::CalculateOrbit(
	const FVector& StartPos, 
	const FVector& EndPos, 
	float ElapsedTime, 
	float BulgeWidth, 
	float BulgeHeight
)
{
	// 直線状の位置
	FVector LinearPos = FMath::Lerp(StartPos, EndPos, ElapsedTime);

	// サイン派でふくらみを計算(中間の値が最大になる)
	float BulgeFactor = FMath::Sin(ElapsedTime * PI); // 0-1-0の波形

	// ふくらみを計算
	// StartPosから見たEndPosの方向ベクトル
	FVector Direction = (EndPos - StartPos).GetSafeNormal();
	// 横方向のふくらみの方向（右手の法則で上ベクトルと直交する方向）
	FVector BulgeDir = FVector::CrossProduct(Direction, FVector::UpVector).GetSafeNormal();
	// 横方向のふくらみ
	FVector Bulge = LinearPos + BulgeDir * BulgeFactor * BulgeWidth;
	// 上方向のふくらみ
	Bulge.Z += BulgeFactor * BulgeHeight; // 上方向のふくらみも追加

	return Bulge;
}

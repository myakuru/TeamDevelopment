#include "SphereCollision.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Tests/ToolMenusTestUtilities.h"

// Sets default values
ASphereCollision::ASphereCollision()
	:	MaxActivationDelay(0.f)
	,	MaxDuration(0.f)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }
}

void ASphereCollision::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!OwnerActor.IsValid())
	{
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASphereCollision::BeginPlay()
{
	Super::BeginPlay();
	
	// 攻撃の最大待ち・有効時間を求める
	for (const auto& Entry : SphereEntries)
	{
		// 最大有効時間
		if (Entry.Duration > MaxDuration)
		{
			MaxDuration = Entry.Duration;
		}

		// 最大待ち時間
		if (Entry.ActivationDelay > MaxActivationDelay)
		{
			MaxActivationDelay = Entry.ActivationDelay;
		}
	}
}

void ASphereCollision::Update(float InDeltaTime)
{
	if (SphereEntries.IsEmpty()) { return; }
	
	// 現在の時間(秒)を取得
	const float CurrentTimeSeconds = GetWorld()->GetTimeSeconds();
	for (int32 i = 0; i < SphereEntries.Num(); ++i)
	{
		if (!SphereComponents.IsValidIndex(i)) { continue; }
		const auto Sphere = SphereComponents[i];
		auto& Entry = SphereEntries[i];
		
		// 攻撃開始時間を経過していないなら処理を飛ばす
		if (CurrentTimeSeconds<JustExecuteTime+Entry.ActivationDelay)
		{
			continue;
		}
		
		// 点滅有効時の当たり判定有効無効切り替え更新処理
		if (Entry.BlinkingHitElemental.bIsBlinking)
		{
			UpdateBlinkingHitCheck(
				Sphere,
				Entry.BlinkingHitElemental,
				CurrentTimeSeconds
				);
		}
		// 通常の当たり判定有効無効切り替え更新処理
		else
		{
			UpdateDefaultHitCheck(
				Sphere,
				Entry,
				CurrentTimeSeconds);
		}
	}
}

void ASphereCollision::SetAllCollisionResponseToChannel(const ECollisionChannel InCollisionChannel,
                                                        const ECollisionResponse InCollisionResponse)
{
	if (SphereEntries.IsEmpty()) { return; }

	for (auto& Entry : SphereComponents)
	{
		if (!IsValid(Entry)) { continue; }
	
		Entry->SetCollisionResponseToChannel(
			InCollisionChannel,
			InCollisionResponse
		);
	}
}

void ASphereCollision::SetAllCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (SphereEntries.IsEmpty()) { return; }

	for (auto& Entry : SphereComponents)
	{
		if (!IsValid(Entry)) { continue; }
	
		Entry->SetCollisionEnabled(InEnabled);
	}
}

void ASphereCollision::UpdateBlinkingHitCheck(
		const TObjectPtr<USphereComponent>& InSphereComponent
	,	FBlinkingHitElemental& OutBlinkingHitElemental
	,	const float InCurrentTime)
{
	// 有効化された時間から何秒経ったかを算出
	const float ElapsedTime = InCurrentTime - JustExecuteTime;
	
	// 経過時間をインターバルで割って、出力された数値が「奇数か偶数」かを取得
	// 0.0f ~ 0.4fなら「0」, 0.5f ~ 1.0fなら「1」...
	const int32 Phase = FMath::FloorToInt(ElapsedTime / OutBlinkingHitElemental.BlinkInterval);
	
	// 現在の点滅のON,OFFを取得(trueがON)
	const bool bCurrentActive = (Phase % 2) == 0;
	
	// ONとOFFが切り替わった瞬間であればコリジョンのON・OFFを切り替える
	if (bCurrentActive != OutBlinkingHitElemental.bPrevBlinking)
	{
		InSphereComponent->SetCollisionEnabled(
			bCurrentActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	}
	
	OutBlinkingHitElemental.bPrevBlinking = bCurrentActive;
}

void ASphereCollision::UpdateDefaultHitCheck(
		const TObjectPtr<USphereComponent>& InSphereComponent
	,	const FSphereElemental& InBlinkingHitElemental
	,	const float InCurrentTime)
{
	const float ActivationTime = JustExecuteTime + InBlinkingHitElemental.ActivationDelay;	// 発動する瞬間の時間
	const float DeactivationTime = ActivationTime + InBlinkingHitElemental.Duration;		// 攻撃を無効化する瞬間の時間
		
	// 有効化されて、まだ無効化されていないか
	const bool bShouldBeActive =
		InCurrentTime >= ActivationTime && InCurrentTime < DeactivationTime;
	
	// 現在の球コンポーネントの状態が有効かどうか
	const bool bIsCurrentActive =
		InSphereComponent->GetCollisionEnabled() != ECollisionEnabled::NoCollision;
	
	// 状態に変化がないなら何もしない
	if (bShouldBeActive == bIsCurrentActive)
	{
		return;
	}
	
	InSphereComponent->SetCollisionEnabled(
		bShouldBeActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ASphereCollision::GeneratedSphereComponents()
{
	for (int32 i = 0; i < SphereEntries.Num(); ++i)
	{
		FName Name = *FString::Printf(TEXT("Sphere_%d"), i);
		USphereComponent* NewSphere = NewObject<USphereComponent>(this, Name);
		
		NewSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		NewSphere->SetSphereRadius(SphereEntries[i].Radius);
		NewSphere->SetRelativeTransform(SphereEntries[i].Transform);
		NewSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AddInstanceComponent(NewSphere); // Componentsパネル/ビューポートに表示
		NewSphere->SetupAttachment(RootComponent);
		NewSphere->RegisterComponent();
		SphereComponents.Add(NewSphere);
		
		// SyncSphereComponents内、Sphere生成の直後に追加
#if WITH_EDITOR
		FName LabelName = *FString::Printf(TEXT("#%d"), i);
		UTextRenderComponent* Label = NewObject<UTextRenderComponent>(this, LabelName);
		Label->SetHiddenInGame(true);
		Label->SetupAttachment(NewSphere);
		Label->SetText(FText::FromName(LabelName));
		Label->SetWorldSize(30.f);
		Label->SetTextRenderColor(FColor::Red);
		Label->SetHorizontalAlignment(EHTA_Center);
		Label->RegisterComponent();

		AddInstanceComponent(Label);
		IndexLabels.Add(Label);
#endif
	}
}

void ASphereCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (auto& Entry:SphereComponents)
	{
		if (IsValid(Entry))
		{
			RemoveInstanceComponent(Entry);
			Entry->DestroyComponent();
		}
	}
	SphereComponents.Empty();
	
	// 既存のラベルも全削除
	for (auto& Label : IndexLabels)
	{
		if (IsValid(Label))
		{
			RemoveInstanceComponent(Label);
			Label->DestroyComponent();
		}
	}
	IndexLabels.Empty();

	// 構造体配列から球コンポーネントを生成
	GeneratedSphereComponents();
}

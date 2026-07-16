#include "SphereCollision.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ASphereCollision::ASphereCollision()
	:	MaxActivationDelay(0.f)
	,	MaxDuration(0.f)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }
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
		const auto& Entry = SphereEntries[i];
		
		const float ActivationTime = JustExecuteTime + Entry.ActivationDelay;	// 発動する瞬間の時間
		const float DeactivationTime = ActivationTime + Entry.Duration;			// 攻撃を無効化する瞬間の時間
		
		// 有効化されて、まだ無効化されていないか
		const bool bShouldBeActive =
			CurrentTimeSeconds >= ActivationTime && CurrentTimeSeconds < DeactivationTime;
	
		// 現在の球コンポーネントの状態が有効かどうか
		const bool bIsCurrentActive =
			Sphere->GetCollisionEnabled() != ECollisionEnabled::NoCollision;
	
		// 状態に変化がないなら何もしない
		if (bShouldBeActive == bIsCurrentActive)
		{
			continue;
		}
	
		Sphere->SetCollisionEnabled(
			bShouldBeActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
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

void ASphereCollision::GeneratedSphereComponents()
{
	for (int32 i = 0; i < SphereEntries.Num(); ++i)
	{
		FName Name = *FString::Printf(TEXT("Sphere_%d"), i);
		USphereComponent* NewSphere = NewObject<USphereComponent>(this, Name);
		NewSphere->SetupAttachment(RootComponent);
		NewSphere->RegisterComponent();
		AddInstanceComponent(NewSphere); // Componentsパネル/ビューポートに表示
		
		NewSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		NewSphere->SetSphereRadius(SphereEntries[i].Radius);
		NewSphere->SetRelativeTransform(SphereEntries[i].Transform);
		NewSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponents.Add(NewSphere);
		
		// SyncSphereComponents内、Sphere生成の直後に追加
#if WITH_EDITOR
		FName LabelName = *FString::Printf(TEXT("#%d"), i);
		UTextRenderComponent* Label = NewObject<UTextRenderComponent>(this, LabelName);
		Label->SetHiddenInGame(false);
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

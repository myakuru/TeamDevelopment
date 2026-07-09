#include "SphereCollision.h"

#include "Components/TextRenderComponent.h"

// Sets default values
ASphereCollision::ASphereCollision()
	: MaxActivationDelay(0.f)
	  , MaxDuration(0.f)
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }

	// 構造体配列から球コンポーネントを生成
	GeneratedSphereComponents();
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
	for (auto& Entry : SphereEntries)
	{
		if (!IsValid(Entry.SphereComponent)) { continue; }

		const float ActivationTime = JustExecuteTime + Entry.ActivationDelay;	// 発動する瞬間の時間
		const float DeactivationTime = ActivationTime + Entry.Duration;			// 攻撃を無効化する瞬間の時間

		// 有効化されて、まだ無効化されていないか
		const bool bShouldBeActive =
			CurrentTimeSeconds >= ActivationTime && CurrentTimeSeconds < DeactivationTime;

		// 現在の球コンポーネントの状態が有効かどうか
		const bool bIsCurrentActive =
			Entry.SphereComponent->GetCollisionEnabled() != ECollisionEnabled::NoCollision;

		// 状態に変化がないなら何もしない
		if (bShouldBeActive == bIsCurrentActive)
		{
			continue;
		}

		Entry.SphereComponent->SetCollisionEnabled(
			bShouldBeActive ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
	}
}

void ASphereCollision::SetAllCollisionResponseToChannel(const ECollisionChannel InCollisionChannel,
                                                        const ECollisionResponse InCollisionResponse)
{
	if (SphereEntries.IsEmpty()) { return; }

	for (auto& Entry : SphereEntries)
	{
		if (!IsValid(Entry.SphereComponent)) { continue; }

		Entry.SphereComponent->SetCollisionResponseToChannel(
			InCollisionChannel,
			InCollisionResponse
		);
	}
}

void ASphereCollision::SetAllCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (SphereEntries.IsEmpty()) { return; }

	for (auto& Entry : SphereEntries)
	{
		if (!IsValid(Entry.SphereComponent)) { continue; }

		Entry.SphereComponent->SetCollisionEnabled(InEnabled);
	}
}

void ASphereCollision::GeneratedSphereComponents()
{
	if (!RootComponent) { return; }

	TSet<TObjectPtr<USphereComponent>> ClaimedComponents;

	for (int32 i = 0; i < SphereEntries.Num(); ++i)
	{
		FSphereElemental& Entry = SphereEntries[i];

		// 未使用 or 重複しているなら新規作成
		bool bNeedsNew = !Entry.SphereComponent || ClaimedComponents.Contains(Entry.SphereComponent);

		if (bNeedsNew)
		{
			FName Name = *FString::Printf(TEXT("Sphere_%d"), i);
			TObjectPtr<USphereComponent> NewSphere = NewObject<USphereComponent>(this, Name);
			NewSphere->SetupAttachment(RootComponent);
			NewSphere->RegisterComponent();
			AddInstanceComponent(NewSphere); // Componentsパネル/ビューポートに表示
			Entry.SphereComponent = NewSphere;
		}

		ClaimedComponents.Add(Entry.SphereComponent);

		// 値を反映
		Entry.SphereComponent->SetSphereRadius(Entry.Radius);
		Entry.SphereComponent->SetRelativeTransform(Entry.Transform);
		Entry.SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

#if WITH_EDITOR
		FName LabelName = *FString::Printf(TEXT("#%d"), i);
		UTextRenderComponent* Label = NewObject<UTextRenderComponent>(this, LabelName);
		Label->SetupAttachment(Entry.SphereComponent);
		Label->SetText(FText::FromName(LabelName));
		Label->SetWorldSize(30.f);
		Label->SetTextRenderColor(FColor::Red);
		Label->SetHorizontalAlignment(EHTA_Center);
		Label->RegisterComponent();

		AddInstanceComponent(Label);
		IndexLabels.Add(Label);
#endif
	}

#if WITH_EDITOR
	// オーナーに登録されている球を全て取得
	TArray<TObjectPtr<USphereComponent>> AllOwnerSpheres;
	GetComponents<USphereComponent>(AllOwnerSpheres);

	// 差分がある球だけを削除
	for (auto& Sphere : AllOwnerSpheres)
	{
		if (!ClaimedComponents.Contains(Sphere))
		{
			RemoveInstanceComponent(Sphere);
			Sphere->DestroyComponent();
		}
	}
#endif
}

void ASphereCollision::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 既存のラベルも全削除
	for (auto& Label : IndexLabels)
	{
		if (Label)
		{
			RemoveInstanceComponent(Label);
			Label->DestroyComponent();
		}
	}
	IndexLabels.Empty();

	// 構造体配列から球コンポーネントを生成
	GeneratedSphereComponents();
}

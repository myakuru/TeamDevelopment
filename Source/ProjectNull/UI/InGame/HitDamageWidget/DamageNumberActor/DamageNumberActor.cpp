//----------------------------------------------------------
//		Includes
//----------------------------------------------------------
#include "DamageNumberActor.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "../DamageNumberWidget/DamageNumberWidget.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/DamageNumberPoolSubsystem/DamageNumberPoolSubsystem.h>

//----------------------------------------------------------
//		コンストラクタ
//----------------------------------------------------------
ADamageNumberActor::ADamageNumberActor()
{
	PrimaryActorTick.bCanEverTick			= true;
	PrimaryActorTick.bStartWithTickEnabled	= false;

	// ルートコンポーネントをしてセット
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// WidgetComponentをルートに設定
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(Root);

	// ダメージ数値はカメラに向いてほしいのでScreen使用
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawSize(FVector2D(200.0f, 80.0f));
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ADamageNumberActor::BeginPlay()
{
	Super::BeginPlay();

	// DamageWidgetClassの有無チェック
	if (DamageWidgetClass)
	{
		WidgetComponent->SetWidgetClass(DamageWidgetClass);
		WidgetComponent->InitWidget();
	}

	// 更新を止める
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
}

void ADamageNumberActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bUsing)
	{
		return;
	}

	CurrentTime += DeltaTime;

	// 上に移動
	AddActorWorldOffset(FVector(0.0f, 0.0f, FloatSpeed * DeltaTime));

	// 一定時間経過で非表示
	if (CurrentTime >= LifeTime)
	{
		// プールに返す
		if (OwnerPool)
		{
			OwnerPool->ReleaseDamageNumber(this);
		}
		else
		{
			DeactivateDamageNumber();
		}
	}
}

//----------------------------------------------------------
//		ダメージ表記用のComponentの有効化
//----------------------------------------------------------
void ADamageNumberActor::ActivateDamageNumber(
	const FVector& WorldLocation,
	int32 Damage,
	bool bCritical,
	UDamageNumberPoolSubsystem* InOwnerPool
)
{
	// プールを設定
	OwnerPool = InOwnerPool;
	CurrentTime = 0.0f;
	bUsing = true;

	// 初期座標に変化を与える
	const FVector RandomOffset = FVector(
		FMath::RandRange(-RandomOffsetRadius, RandomOffsetRadius),
		FMath::RandRange(-RandomOffsetRadius, RandomOffsetRadius),
		FMath::RandRange(0.0f, RandomOffsetRadius)
	);

	SetActorLocation(WorldLocation + RandomOffset);	// 座標のセット
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
	SetActorTickEnabled(true);						// Tickの有効化

	if (WidgetComponent)
	{
		// WidgetComponentの表示
		WidgetComponent->SetVisibility(true, true);
		WidgetComponent->InitWidget();

		// Componentの実体の取得
		UDamageNumberWidget* DamageWidget =
			Cast<UDamageNumberWidget>(WidgetComponent->GetUserWidgetObject());

		// DamageWidgetを通して表示するダメージの設定とアニメーション更新
		if (DamageWidget)
		{
			DamageWidget->SetDamageValue(Damage, bCritical);
			DamageWidget->PlayPopupAnimation();
		}
	}
}

//----------------------------------------------------------
//		ダメージ表記用のComponentの無効化
//----------------------------------------------------------
void ADamageNumberActor::DeactivateDamageNumber()
{
	// 終了処理
	bUsing = false;
	CurrentTime = 0.0f;
	OwnerPool = nullptr;


	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(false, true);
	}
}


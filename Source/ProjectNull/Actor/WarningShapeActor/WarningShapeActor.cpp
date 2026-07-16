#include "WarningShapeActor.h"

// Sets default values
AWarningShapeActor::AWarningShapeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!IsValid(RootComponent)) { return; }

	TargetScaleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TargetScaleMeshComponent");
	if (!IsValid(TargetScaleMeshComponent)) { return; }
	TargetScaleMeshComponent->SetupAttachment(RootComponent);

	VariableScaleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("VariableScaleMeshComponent");
	if (!IsValid(VariableScaleMeshComponent)) { return; }
	VariableScaleMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWarningShapeActor::BeginPlay()
{
	Super::BeginPlay();

	// 初期状態は非表示にしておく
	SetActorHiddenInGame(true);

	TargetScaleMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	VariableScaleMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	VariableScaleMeshComponent->SetWorldScale3D(FVector::ZeroVector);
}

void AWarningShapeActor::DeActivate()
{
	// 非表示
	SetActorHiddenInGame(true);

	// スケール値リセット
	VariableScaleMeshComponent->SetWorldScale3D(FVector::ZeroVector);
}

void AWarningShapeActor::Initialize()
{
	// スタティックメッシュのバウンス(ローカル)を取得
	// UEのデフォルト球なら50.0fになる
	FBoxSphereBounds Bounds = TargetScaleMeshComponent->GetStaticMesh()->GetBounds();

	// スケールの倍率を計算
	float MeshOriginalRadius = Bounds.SphereRadius;
	ScaleFactor = TargetRadius / MeshOriginalRadius;

	// 目標スケール値をセット
	TargetScaleMeshComponent->SetWorldScale3D(FVector(ScaleFactor));
}

void AWarningShapeActor::Execute()
{
	// 表示
	SetActorHiddenInGame(false);

	DurationTimer = 0.f;
}

void AWarningShapeActor::Update(float DeltaTime)
{
	float Alpha = FMath::Clamp(DurationTimer / ActivationDelay, 0.f, 1.f);

	// 補間が終了したら無効化
	if (Alpha >= 1.f)
	{
		DeActivate();
	}

	FVector Scale = FMath::Lerp(FVector::ZeroVector, TargetScaleMeshComponent->GetRelativeScale3D(), Alpha);
	VariableScaleMeshComponent->SetWorldScale3D(Scale);
	DurationTimer += DeltaTime;
}

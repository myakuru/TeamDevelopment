
#include "GhostActor.h"

#include <ProjectNull/System/AnimInstance/GhostActorAnimInstance/GhostActorAnimInstance.h>

AGhostActor::AGhostActor():
	TranslucencySortPriority(10),
	LifeTime(0.5f),
	CurrentTime(0.0f),
	StartOpacity(1.0f)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (!Mesh) { return; }
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCastShadow(false);
}

void AGhostActor::BeginPlay()
{
	Super::BeginPlay();
	Opacity = StartOpacity;
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!SkeletalMesh) { return; }

	// SkeletalMesh設定
	Mesh->SetSkeletalMesh(SkeletalMesh);

	// Animation設定
	if (!Mesh->GetAnimInstance()) { return; }

	auto* GhostAnimInstance = Cast<UGhostActorAnimInstance>(Mesh->GetAnimInstance());
	if (!GhostAnimInstance) { return; }

	GhostAnimInstance->GhostPoseSnapshot = InSnapshot;

	// アニメ停止
	Mesh->SetRenderCustomDepth(true);
	Mesh->TranslucencySortPriority = TranslucencySortPriority;

	const int32 MaterialNum = Mesh->GetNumMaterials();

	for (int32 Num = 0; Num < MaterialNum; Num++)
	{
		if (!GhostMaterial) { continue; }

		auto* DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterial, this);

		Mesh->SetMaterial(Num, DynamicMaterial);
		DynamicMaterials.Add(DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), StartOpacity);
		DynamicMaterial->SetVectorParameterValue(TEXT("Rim_Color"), StartRimColor);
	}
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, const FPoseSnapshot& InSnapshot, float InLifeTime, float InOpacityDecayRate)
{
	if (!SkeletalMesh) { return; }

	LifeTime			= InLifeTime;
	OpacityDecayRate	= InOpacityDecayRate;

	// SkeletalMesh設定
	Mesh->SetSkeletalMesh(SkeletalMesh);

	// Animation設定
	if (!Mesh->GetAnimInstance()) { return; }

	auto* GhostAnimInstance = Cast<UGhostActorAnimInstance>(Mesh->GetAnimInstance());
	if (!GhostAnimInstance) { return; }

	GhostAnimInstance->GhostPoseSnapshot = InSnapshot;

	// アニメ停止
	Mesh->SetRenderCustomDepth(true);
	Mesh->TranslucencySortPriority = TranslucencySortPriority;

	const int32 MaterialNum = Mesh->GetNumMaterials();

	for (int32 Num = 0; Num < MaterialNum; Num++)
	{
		if (!GhostMaterial) { continue; }

		auto* DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterial, this);

		Mesh->SetMaterial(Num, DynamicMaterial);
		DynamicMaterials.Add(DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), StartOpacity);
		DynamicMaterial->SetVectorParameterValue(TEXT("Rim_Color"), StartRimColor);
	}
}

void AGhostActor::Update(float DeltaTime)
{
	CurrentTime += DeltaTime;
	Opacity -= OpacityDecayRate * DeltaTime;

	for (UMaterialInstanceDynamic* Material : DynamicMaterials) {
		if (!Material) { continue; }

		Material->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}

	if (CurrentTime >= LifeTime) {
		Destroy();
	}
}


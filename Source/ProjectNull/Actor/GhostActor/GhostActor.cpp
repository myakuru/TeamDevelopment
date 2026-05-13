
#include "GhostActor.h"

AGhostActor::AGhostActor():
	TranslucencySortPriority(10),
	LifeTime(0.5f),
	CurrentTime(0.0f),
	StartOpacity(1.0f),
	StartColor(FLinearColor::Black),
	StartBaseColor(FLinearColor::Black)
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (!Mesh) { return; }
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCastShadow(false);
}

void AGhostActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGhostActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;

	const float Alpha = 1.0f - (CurrentTime / LifeTime);

	for (UMaterialInstanceDynamic* Material : DynamicMaterials) {
		if (!Material) { continue; }

		Material->SetScalarParameterValue(TEXT("Opacity"), Alpha);
	}

	if (CurrentTime >= LifeTime) {
		Destroy();
	}

}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, UAnimationAsset* Animation, float PoseTime)
{
	if (!SkeletalMesh || !Animation || !Mesh) { return; }

	// SkeletalMesh設定
	Mesh->SetSkeletalMesh(SkeletalMesh);

	// SingleNodeモード
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	// Animation設定
	Mesh->SetAnimation(Animation);

	// 特定時間へ移動
	Mesh->SetPosition(PoseTime);

	// アニメ停止
	Mesh->bPauseAnims = true;
	Mesh->SetRenderCustomDepth(true);
	Mesh->TranslucencySortPriority = TranslucencySortPriority;

	const int32 MaterialNum = Mesh->GetNumMaterials();

	for (int32 Num = 0; Num < MaterialNum; Num++)
	{
		if (!GhostMaterial) { continue; }

		auto* DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterial,this);

		Mesh->SetMaterial(Num, DynamicMaterial);
		DynamicMaterials.Add(DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"),StartOpacity);
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"),StartColor);
		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), StartBaseColor);
	}
}

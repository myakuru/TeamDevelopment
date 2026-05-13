
#include "GhostActor.h"

AGhostActor::AGhostActor()
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

	float Alpha =
		1.0f - (CurrentTime / LifeTime);

	for (UMaterialInstanceDynamic* Material :
		DynamicMaterials)
	{
		if (!Material)
		{
			continue;
		}

		Material->SetScalarParameterValue(
			TEXT("Opacity"),
			Alpha
		);
	}

	if (CurrentTime >= LifeTime)
	{
		Destroy();
	}

}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, UAnimationAsset* Animation, TObjectPtr<UMaterialInterface> GhostMaterial, float PoseTime)
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
	Mesh->TranslucencySortPriority = 10;

	const int32 MaterialNum = Mesh->GetNumMaterials();

	for (int32 Num = 0; Num < MaterialNum; Num++)
	{
		if (!GhostMaterial)
		{
			continue;
		}

		UMaterialInstanceDynamic* DynamicMaterial =
			UMaterialInstanceDynamic::Create(
				GhostMaterial,
				this
			);

		Mesh->SetMaterial(Num, DynamicMaterial);

		DynamicMaterials.Add(DynamicMaterial);

		// 初期色
		DynamicMaterial->SetScalarParameterValue(
			TEXT("Opacity"),
			0.5f
		);
		DynamicMaterial->SetVectorParameterValue(
			TEXT("Color"),
			FLinearColor(0, 1, 1, 1)
		);
	}
}

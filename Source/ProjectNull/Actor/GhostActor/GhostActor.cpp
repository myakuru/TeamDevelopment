
#include "GhostActor.h"

#include <ProjectNull/System/AnimInstance/GhostActorAnimInstance/GhostActorAnimInstance.h>

AGhostActor::AGhostActor():
	Root(nullptr),
	Mesh(nullptr),
	GhostMaterial(nullptr),
	DynamicMaterials(TArray<TObjectPtr<UMaterialInstanceDynamic>>()),
	TranslucencySortPriority(10),
	LifeTime(0.5f),
	CurrentTime(0.f),
	StartOpacity(1.f),
	OpacityDecayRate(1.f),
	Opacity(1.f),
	StartRimColor(FLinearColor::Black),
	RimSharpness(0.f),
	RimStrength(1.f)
{
	// ================================================================
	// 残像クラスの初期化
	// ================================================================
	PrimaryActorTick.bCanEverTick = true;

	// ================================================================
	// ルートコンポーネントの初期化
	// ================================================================
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// ================================================================
	// スケルタルメッシュの初期化
	// ================================================================
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (!Mesh) { return; }
	Mesh->SetupAttachment(Root);

	// コリジョン無効
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 影の描画無効
	Mesh->SetCastShadow(false);
}

void AGhostActor::BeginPlay()
{
	Super::BeginPlay();

	// 透明度の初期化
	Opacity = StartOpacity;
}

void AGhostActor::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	// 時間加算処理
	CurrentTime += DeltaTime;

	// 透明度減少処理
	Opacity -= OpacityDecayRate * DeltaTime;

	// マテリアルへ動的に透明度を更新
	for (auto& Material : DynamicMaterials) {
		if (!Material) { continue; }
		Material->SetScalarParameterValue(TEXT("Opacity"), Opacity);
	}

	// 寿命時間または透明度が透明になったらワールドから削除
	if (CurrentTime >= LifeTime || Opacity <= 0.0f) {
		Destroy();
	}
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!SkeletalMesh) { return; }

	// 共通初期化処理
	InitializeInternal(SkeletalMesh);

	if (!Mesh->GetAnimInstance()) { return; }

	// 残像クラス用アニメーションインスタンス取得する
	auto* GhostAnimInstance = Cast<UGhostActorAnimInstance>(Mesh->GetAnimInstance());
	if (!GhostAnimInstance) { return; }
	
	// ポーズを更新
	GhostAnimInstance->GhostPoseSnapshot = InSnapshot;
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, const FPoseSnapshot& InSnapshot, float InLifeTime, float InOpacityDecayRate)
{
	if (!SkeletalMesh || !Mesh) { return; }

	// 寿命時間
	LifeTime			= InLifeTime;

	// 透明度減少量
	OpacityDecayRate	= InOpacityDecayRate;

	// 共通初期化処理
	InitializeInternal(SkeletalMesh);

	// 残像クラス用アニメーションインスタンス取得する
	if (!Mesh->GetAnimInstance()) { return; }

	// 残像クラス用アニメーションインスタンス取得する
	auto* GhostAnimInstance = Cast<UGhostActorAnimInstance>(Mesh->GetAnimInstance());
	if (!GhostAnimInstance) { return; }

	// ポーズを更新
	GhostAnimInstance->GhostPoseSnapshot = InSnapshot;
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, UAnimationAsset* Animation, float InPoseTime)
{
	if (!SkeletalMesh || !Mesh) { return; }

	// 共通初期化処理
	InitializeInternal(SkeletalMesh);

	// アニメーションモードをシングルノードに変更し、
	// アニメーションアセットと時間閾値をセット
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	Mesh->SetAnimation(Animation);
	Mesh->SetPosition(InPoseTime);

	// アニメーション停止
	Mesh->bPauseAnims = true;
}

void AGhostActor::Initialize(USkeletalMesh* SkeletalMesh, UAnimationAsset* Animation, float InPoseTime, float InLifeTime, float InOpacityDecayRate)
{
	if (!SkeletalMesh || !Mesh) { return; }

	// 寿命時間
	LifeTime			= InLifeTime;

	// 透明度減少量
	OpacityDecayRate	= InOpacityDecayRate;

	// 共通初期化処理
	InitializeInternal(SkeletalMesh);

	// アニメーションモードをシングルノードに変更し、
	// アニメーションアセットと時間閾値をセット
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	Mesh->SetAnimation(Animation);
	Mesh->SetPosition(InPoseTime);

	// アニメーション停止
	Mesh->bPauseAnims = true;
}

void AGhostActor::InitializeInternal(USkeletalMesh* SkeletalMesh)
{
	if (!SkeletalMesh || !Mesh) { return; }

	// スケルタルメッシュの設定
	Mesh->SetSkeletalMesh(SkeletalMesh);

	// スケルタルメッシュの深度情報設定
	Mesh->SetRenderCustomDepth(true);

	// 透過度のソート優先度
	Mesh->TranslucencySortPriority = TranslucencySortPriority;

	// スケルタルメッシュのマテリアル数取得
	const int32 MaterialNum = Mesh->GetNumMaterials();

	// 動的に残像マテリアルを生成し、スケルタルメッシュのすべてのマテリアルに設定を行う
	for (int32 Num = 0; Num < MaterialNum; Num++)
	{
		if (!GhostMaterial) { continue; }

		auto* DynamicMaterial = UMaterialInstanceDynamic::Create(GhostMaterial, this);

		Mesh->SetMaterial(Num, DynamicMaterial);
		DynamicMaterials.Add(DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), StartOpacity);
		DynamicMaterial->SetVectorParameterValue(TEXT("Rim_Color"), StartRimColor);
		DynamicMaterial->SetScalarParameterValue(TEXT("Rim_Sharpness"), RimSharpness);
		DynamicMaterial->SetScalarParameterValue(TEXT("Rim_Strength"), RimStrength);
	}
}

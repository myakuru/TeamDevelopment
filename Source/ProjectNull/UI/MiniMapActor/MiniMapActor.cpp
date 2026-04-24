// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapActor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

// Sets default values
AMiniMapActor::AMiniMapActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �L���v�`���R���|�[�l���g�̍쐬
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	RootComponent = SceneCaptureComponent;

	// �^�������
	SceneCaptureComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// ========================================
	// �~�j�}�b�v�œK���ݒ�
	// ========================================

	// ���e�ݒ� - �����e�iOrthographic�j�Ńt���b�g�ȕ\��
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;

	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	// �`��ݒ� - �ȈՓI�ȕ\��
	SceneCaptureComponent->ShowFlags.SetFog(false);              // �t�H�O����
	SceneCaptureComponent->ShowFlags.SetAtmosphere(false);       // ��C����
	SceneCaptureComponent->ShowFlags.SetVolumetricFog(false);    // �{�������g���b�N�t�H�O����
	SceneCaptureComponent->ShowFlags.SetMotionBlur(false);       // ���[�V�����u���[����
	SceneCaptureComponent->ShowFlags.SetBloom(false);            // �u���[������
	SceneCaptureComponent->ShowFlags.SetAmbientOcclusion(false); // �A���r�G���g�I�N���[�W��������
	SceneCaptureComponent->ShowFlags.SetAntiAliasing(false);     // �A���`�G�C���A�X�����i�y�ʉ��j
	SceneCaptureComponent->ShowFlags.SetEyeAdaptation(false);    // �ڂ̏�������
	SceneCaptureComponent->ShowFlags.SetTemporalAA(false);       // TAA����

	// ========================================
	// �e����S�ɖ�����
	// ========================================
	SceneCaptureComponent->ShowFlags.SetDynamicShadows(false);      // ���I�e����
	SceneCaptureComponent->ShowFlags.SetContactShadows(false);      // �R���^�N�g�V���h�E����
	SceneCaptureComponent->ShowFlags.SetCapsuleShadows(false);      // �J�v�Z���V���h�E����
	SceneCaptureComponent->ShowFlags.SetShadowFrustums(false);      // �V���h�E�t���X�^������

	// ���C�e�B���O��ȑf���i�I�v�V�����F���S�t���b�g�ɂ������ꍇ��Lighting��������j
	SceneCaptureComponent->ShowFlags.SetLighting(false); // ���C�e�B���O���S����
	SceneCaptureComponent->ShowFlags.SetReflectionEnvironment(false); // ���˖���
	SceneCaptureComponent->ShowFlags.SetGlobalIllumination(false);    // GI����

	// �p�[�e�B�N����f�J�[���̕\���i�K�v�ɉ����Ē����j
	SceneCaptureComponent->ShowFlags.SetParticles(false);  // �p�[�e�B�N���\��
	SceneCaptureComponent->ShowFlags.SetDecals(false);    // �f�J�[����\��

	// LOD�ݒ� - �Œ�LOD��g�p���ăp�t�H�[�}���X����
	SceneCaptureComponent->ShowFlags.SetLOD(false);

	// ���̑��̍œK��
	SceneCaptureComponent->ShowFlags.SetScreenSpaceReflections(false); // SSR����
	SceneCaptureComponent->ShowFlags.SetAmbientCubemap(false);         // �A���r�G���g�L���[�u�}�b�v����
	SceneCaptureComponent->ShowFlags.SetDistanceFieldAO(false);        // DFAO����

	// ========================================
	// �I�v�V����: ����̃A�N�^�[�̂ݕ\��
	// ========================================
	// Unreal Engine�́uShow Only Actors�v�@�\��g�p����ꍇ
	// �G�f�B�^�܂��̓R�[�h��ShowOnlyComponents��ݒ�\

	TargetCharacter = nullptr;

}

// Called when the game starts or when spawned
void AMiniMapActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMiniMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter)
	{
		FVector CharacterLocation = TargetCharacter->GetActorLocation();

		// �J�����̍�����l�����ĐV�����ʒu��v�Z
		FVector NewLocation = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z + CameraHeight);

		// �J�����̈ʒu��X�V
		SetActorLocation(NewLocation);
	}
	else
	{
		// �L�����N�^�[��������Ȃ��ꍇ�͍Ď擾����݂�
		TargetCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

}


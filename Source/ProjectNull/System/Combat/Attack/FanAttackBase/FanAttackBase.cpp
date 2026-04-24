// Fill out your copyright notice in the Description page of Project Settings.


#include "FanAttackBase.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UFanAttackBase::UFanAttackBase()
	: bRotate(false)
	, Duration(1.0f)
	, ElapsedTime(0.0f)
	, RotationSpeed(1000.0f)
	, Radius(300.0f)
	, ConeAngle(30.0f)
	, bIsActive(false)
	, bPrevActive(false)
	, CurrentAngle(0.0f)
	, KnockbackPower(2.0f)
	, StartAngle(0.0f)
{
}

void UFanAttackBase::Start()
{
	bIsActive		= true;
	CurrentAngle	= StartAngle;
	ElapsedTime		= 0.0f;
}

bool UFanAttackBase::UpdateAttack(float DeltaTime)
{
	if (!bIsActive) { return false; }

	ElapsedTime += DeltaTime;

	// ��]����
	if (bRotate)
	{
		CurrentAngle += RotationSpeed * DeltaTime;
	}

	// �U���͈͂�f�o�b�O���C���ŉ���
	{
		// �v���C���[�̍��W�ƑO���x�N�g����擾
		const FVector playerLocation = OwnerActor->GetActorLocation();
		const FVector forwardVector = OwnerActor->GetActorForwardVector();

		// �U�������x�N�g��
		const FVector attackDir = CalcAttackDir(forwardVector);

		UDebugDrawLibrary::DrawDebugFan(
			GetWorld(),
			playerLocation,
			attackDir,
			Radius,
			ConeAngle,
			10
		);
	}

	// �I������
	if (ElapsedTime >= Duration)
	{
		bIsActive = false;
	}

	return true;
}

bool UFanAttackBase::IsTargetInRange(AActor* Target, const FVector& OwnerLocation)
{
	if (!OwnerActor) { return false; }
	if (!Target) { return false; }

	// �G�ւ̃x�N�g��
	FVector ToEnemy = Target->GetActorLocation() - OwnerLocation;

	// �U�������x�N�g��
	const FVector AttackDir = CalcAttackDir(OwnerActor->GetActorForwardVector());

	// �����`�F�b�N
	if (ToEnemy.SizeSquared() > GetRadiusSquared())
	{
		return false;
	}

	// �x�N�g�����K��
	ToEnemy.Normalize();

	// �p�x�`�F�b�N
	float Dot = FVector::DotProduct(AttackDir, ToEnemy);

	return Dot > GetConeCosine();
}


void UFanAttackBase::AttackJudgePlayer(AActor* Player)
{
	if (!Player) { return; }

	// ������̍��W��擾
	const FVector ownerLocation = OwnerActor->GetActorLocation();

	// �G���U���͈͓�ɂ��邩����
	if (IsTargetInRange(Player, ownerLocation))
	{
		// �_���[�W��^����(������)
	}
}

void UFanAttackBase::AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager)
{
	if (!EnemyManager) { return; }

	// �v���C���[�̍��W��擾
	const FVector playerLocation = OwnerActor->GetActorLocation();

	// �G���X�g����[�v���āA�U���͈͓�̓G�Ƀ_���[�W��^����
	for (auto& enemy : EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		// �G���U���͈͓�ɂ��邩����
		if (IsTargetInRange(enemy, playerLocation))
		{
			// ���������v���P(�m�b�N�o�b�N�̋����Ȃǂ͊��ɒu�����Q�b�g�ł���悤�ɂ�����)
			enemy->SetKnockBackData(playerLocation, 2.0f, 1.0f);

			// �_���[�W��^����
			enemy->SetTakeDamaged();
		}
	}
}

bool UFanAttackBase::CanDeactivate()
{
	bool canDeactivate = (bIsActive != bPrevActive) && !bIsActive;
	bPrevActive = bIsActive;
	return canDeactivate;
}

bool UFanAttackBase::IsActiveFirstFrame()
{
	bool canDeactivate = (bIsActive != bPrevActive) && bIsActive;
	bPrevActive = bIsActive;
	return canDeactivate;
}

void UFanAttackBase::UpdatePrevActiveFlg()
{
	bPrevActive = bIsActive;
}

FVector UFanAttackBase::CalcAttackDir(const FVector& forwardVector) const
{
	const float angle = bRotate ? CurrentAngle : 0.0f;
	return forwardVector.RotateAngleAxis(angle, FVector::UpVector);
}
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameProgressSubsystem.generated.h"

/// <summary>
/// �Q�[���̐i�s�Ǘ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UGameProgressSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// �G��|�����ۂɓ|�����G������Z
	/// </summary>
	/// <param name="Value">���Z�������G��</param>
	void AddKillCount(int32 Value = 1);

	int32 GetKillCount() const { return KillCount; }

	int32 GetPhase() const { return Phase; }

	/// <summary>
	/// �t�F�[�Y�̋��E���`����|�����G��臒l
	/// </summary>
	UPROPERTY(EditAnywhere)
	TArray<int32> PhaseThresholds;

private:

	/// <summary>
	/// �G��|������
	/// </summary>
	int32 KillCount = 0;

	/// <summary>
	///	�Q�[���̃t�F�[�Y��
	/// </summary>
	int32 Phase = 0;

	/// <summary>
	/// �Q�[���̃t�F�[�Y�X�V
	/// �����t���[���Ăԃ��\�b�h�ł͂Ȃ�
	/// </summary>
	void UpdatePhase();
};

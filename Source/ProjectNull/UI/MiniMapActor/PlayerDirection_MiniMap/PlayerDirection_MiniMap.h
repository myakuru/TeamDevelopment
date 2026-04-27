// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDirection_MiniMap.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UPlayerDirection_MiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:

	// �~�j�}�b�v�̃v���C���[���������Image
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> PlayerDirection;

	// �������֐�
	virtual void NativeConstruct() override;

	// Tick������L���ɂ��邽�߂̃I�[�o�[���C�h
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	// �v���C���[�̕�����X�V����֐�
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerDirection(float PlayerYaw);
	
};

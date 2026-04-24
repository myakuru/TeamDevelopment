// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSearchArea_MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UPlayerSearchArea_MiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:

	// �~�j�}�b�v�̃v���C���[���������Image
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* PlayerSearchArea;

	// �������֐�
	virtual void NativeConstruct() override;

	// Tick������L���ɂ��邽�߂̃I�[�o�[���C�h
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	// �v���C���[�̕�����X�V����֐�
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerSearchArea(float CameraYaw);
	
};

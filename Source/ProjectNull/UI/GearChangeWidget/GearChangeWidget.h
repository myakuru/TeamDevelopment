// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GearChangeWidget.generated.h"

class UImage;
class UTextBlock;

/**
 * ���̃N���X�́A�M�A�`�F���W��UI��Ǘ����邽�߂̃E�B�W�F�b�g�N���X�ł��B�M�A�`�F���W�̍ۂɕ\�������UI�v�f�𐧌䂵�A�K�v�ɉ����ă}�e���A����C���[�W��X�V��������S���܂��B
 */
UCLASS()
class PROJECTNULL_API UGearChangeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	// �M�A�`�F���W�p��UI�}�e���A��
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> GearChangeMaterial;

	// �M�A�`�F���W�p��UI�C���[�W
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GearChangeImage;

	// �M�A�`�F���W�̃`���[�W�ʂ̃e�L�X�g
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> GearChangeText;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UFUNCTION(BlueprintCallable)
	void SetGearChangeEnergy(float Charge);
	
};

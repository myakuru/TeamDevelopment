// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMapActor.generated.h"

UCLASS()
class PROJECTNULL_API AMiniMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ��ʂ�L���v�`�����邽�߂̃R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	// �L�����N�^�[�̒ǐ�
	UPROPERTY()
	class ACharacter* TargetCharacter;

	// �J�����̍���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float CameraHeight = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

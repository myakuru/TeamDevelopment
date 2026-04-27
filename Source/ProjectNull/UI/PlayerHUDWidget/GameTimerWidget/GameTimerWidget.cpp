// Fill out your copyright notice in the Description page of Project Settings.
#include "GameTimerWidget.h"
#include "TimerManager.h"

void UGameTimerWidget::StartTimer(float Duration)
{
	RemainingTime = Duration;

	// ���[���h����^�C�}�[��擾���āACountDownTimer�֐�����Ԋu�ŌĂяo��
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UGameTimerWidget::CountDownTimer, 1.0f, true);

}

void UGameTimerWidget::CountDownTimer()
{
	if (RemainingTime <= 0)
	{
		// �^�C�}�[���I�������ꍇ�̏���
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		TimerText = TEXT("Time's Up!");
	}
	else
	{
		// �^�C�}�[���܂��c���Ă���ꍇ�̏���
		int32 Hours = FMath::FloorToInt(RemainingTime / 3600);
		int32 Minutes = FMath::FloorToInt(RemainingTime / 60);
		int32 Seconds = FMath::FloorToInt(RemainingTime) % 60;
		TimerText = FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
		RemainingTime -= 1.0f; // 1�b���炷
	}
}


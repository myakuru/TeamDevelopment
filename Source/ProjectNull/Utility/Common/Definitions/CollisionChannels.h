// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

/**
 * @brief 便利なマクロ定義(ヒューマンエラーを防ぐために)
 */
#define ECC_Enemy	ECollisionChannel::ECC_GameTraceChannel1	// 「敵」のコリジョンチャンネル
#define ECC_Player	ECollisionChannel::ECC_GameTraceChannel2	// 「プレイヤー」のコリジョンチャンネル
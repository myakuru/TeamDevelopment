#include "SphereAttack.h"

#include "Components/SphereComponent.h"

#include <ProjectNull/Actor/WarningShapeActor/WarningShapeActor.h>
#include <ProjectNull/Actor/CollisionActor/SphereCollision/SphereCollision.h>

USphereAttack::USphereAttack()
{
}

void USphereAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	if (!InOwner) { return; }

	UAttackBase::Initialize(InOwner);

	// 攻撃用のSphereCollisionアクターの設定
	{
		SphereCollision = GetWorld()->SpawnActor<ASphereCollision>(SubSphereCollision);
		if (!SphereCollision) { return; }

		// 攻撃用スフィアアクターを親にアタッチ
		SphereCollision->AttachToComponent(
			GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 攻撃の最大時間を加算して「攻撃の最大有効時間」として使う
		AddDuration(SphereCollision->GetBaseAttackDuration());

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		SphereCollision->SetAllCollisionResponseToChannel(
			GetTargetChannel(),
			GetTargetResponse()
		);

		// オーバーラップ時、オーバーラップ抜け時の関数をセット
		for (const auto& Sphere : SphereCollision->GetSphereComponents())
		{
			if (!IsValid(Sphere)) { continue; }
			Sphere->OnComponentBeginOverlap.AddDynamic(
					this
				,	&ThisClass::OnCollisionBeginOverlap
			);
			
			Sphere->OnComponentEndOverlap.AddDynamic(
					this
				,	&ThisClass::OnCollisionEndOverlap
			);
		}

		Cancel();
	}
	
	// 警告を出力するなら専用アクターを生成
	if (IsShowWarning)
	{
		for (int32 i = 0; i < SphereCollision->GetSphereComponents().Num(); ++i)
		{
			// 初期値パラメータをセットしておく
			if (SphereCollision->GetSphereEntries().IsValidIndex(i))
			{
				const FSphereElemental& Sphere = SphereCollision->GetSphereEntries()[i];
				TObjectPtr<AWarningShapeActor> WarningShape = GetWorld()->SpawnActor<AWarningShapeActor>(WarningShapeActor);
				WarningShape->AttachToComponent(SphereCollision->GetSphereComponents()[i],FAttachmentTransformRules::KeepRelativeTransform);
				
				// 実行までの時間をセット
				WarningShape->SetActivationDelay(Sphere.ActivationDelay);
				if (const auto& SphereComponent=SphereCollision->GetSphereComponents()[i])
				{
					// 目標半径
					WarningShape->SetTargetRadius(
						SphereComponent->GetScaledSphereRadius());
				}
				
				WarningShape->Initialize();
				// 要素を追加
				WarningShapes.Add(WarningShape);
			}
		}
	}
}
void USphereAttack::Execute(const FVector& InTargetLocation)
{
	UCollisionAttack::Execute(InTargetLocation);
	
	if (!SphereCollision) { return; }
	SphereCollision->SetJustExecuteTime(GetJustExecuteTime());			// 有効化された瞬間の時間をセット
	SphereCollision->SetActorRelativeTransform(GetOffsetTransform());	// 補正用トランスフォーム値をコリジョンの基準としてセット
	
	// 警告を出力するなら実行
	if (IsShowWarning)
	{
		for (const auto& Warning:WarningShapes)
		{
			Warning->Execute();
		}
	}
}

void USphereAttack::Cancel()
{
	UCollisionAttack::Cancel();

	if (!SphereCollision) { return; }
	SphereCollision->SetAllCollisionEnabled(ECollisionEnabled::NoCollision); // 当たり判定無効化
}

void USphereAttack::Update(float InDeltaTime)
{
	UCollisionAttack::Update(InDeltaTime);
	
	if (!IsActive()) { return; }

	if (!IsValid(SphereCollision)) { return; }
	SphereCollision->Update(InDeltaTime);
	
	// 警告を出力するなら更新
	if (IsShowWarning)
	{
		for (const auto& Warning:WarningShapes)
		{
			Warning->Update(InDeltaTime);
		}
	}
}

void USphereAttack::ApplyCollisionTransform(const FTransform& InTransform)
{
	if (!IsValid(SphereCollision)) { return; }
	
	SphereCollision->SetActorTransform(InTransform);
}
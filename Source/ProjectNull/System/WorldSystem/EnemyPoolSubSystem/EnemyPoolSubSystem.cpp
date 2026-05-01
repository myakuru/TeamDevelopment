#include "EnemyPoolSubSystem.h"
#include "EnemyPoolConfig.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

void UEnemyPoolSubSystem::WarmUp(UEnemyPoolConfig* InPoolConfig)
{
    // EnemyClassが未設定なら何もしない
    if (!InPoolConfig || !InPoolConfig->EnemyClass) { return; }

    UE_LOG(LogTemp, Warning, TEXT("WarmUp In"));

    // EnemyDataが未設定の場合はエラーログを出して終了する
    // EnemyDataが空の場合はSpawn時にActivateでクラッシュする
    if (!InPoolConfig->EnemyData)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[EnemyPool] WarmUp Failure"),
            *InPoolConfig->GetName());
        return;
    }

    FPrimaryAssetId Key = InPoolConfig->GetPrimaryAssetId();

    // すでに同じPoolConfigでWarmUp済みならスキップする
    // 同じConfigが複数フェーズウェーブに登録されていても
    // Actorが二重に生成されることを防ぐ
    if (Pools.Contains(Key))
    {
        UE_LOG(LogTemp, Log,
            TEXT("[EnemyPool] WarmUp Success"), *InPoolConfig->GetName());
        return;
    }

    // Poolを新規作成してPoolNum体分Actorを生成する
    FEnemyPool& Pool = Pools.FindOrAdd(Key);

    for (int32 i = 0; i < InPoolConfig->PoolSize; ++i)
    {
        AEnemyBase* Enemy = CreateNewEnemy(InPoolConfig);
        if (!Enemy) { continue; }

        // 生成したActorを非表示・Tick停止で待機状態にする
        Enemy->Deactivate();
        Pool.Inactive.Add(Enemy);
        // Return（）時にどのPoolに戻すか逆引きできるようにマップに登録する
        EnemyToConfig.Add(Enemy, InPoolConfig);
    }

    UE_LOG(LogTemp, Warning, TEXT("WarmUp Out"));
}

AEnemyBase* UEnemyPoolSubSystem::Spawn(UEnemyPoolConfig* InPoolConfig, const FVector& Location)
{
    if (!InPoolConfig) { return nullptr; }

    UE_LOG(LogTemp, Warning, TEXT("Spawn In"));

    // EnemyDataが未設定の場合はSpawnしない
    // ここがnullだとActivate内のcheckでクラッシュするため事前にはじく
    if (!InPoolConfig->EnemyData)
    {
        UE_LOG(LogTemp, Error,
            TEXT("[EnemyPool] Spawn Failure"),
            *InPoolConfig->GetName());
        return nullptr;
    }

    FPrimaryAssetId Key = InPoolConfig->GetPrimaryAssetId();
    FEnemyPool* Pool = Pools.Find(Key);

    // WarmUpが呼ばれていない場合はnullptrが返る
    if (!Pool)
    {
        UE_LOG(LogTemp, Error,
            TEXT("Pool not found for %s. Call WarmUp first."), *InPoolConfig->GetName());
        return nullptr;
    }

    // Inactiveが空の場合はPoolNum不足
    if (Pool->Inactive.IsEmpty())
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Pool exhausted for %s. Consider increasing PoolSize."), *InPoolConfig->GetName());
        return nullptr;
    }

    // InactiveからActorを取り出してActivateする
    AEnemyBase* Enemy = Pool->Inactive.Pop();

    // EnemyDataをActivateに渡す
    // InConfig->EnemyDataはこの時点でnullチェック済みなので安全
    Enemy->Activate(Location, InPoolConfig->EnemyData);

    UE_LOG(LogTemp, Warning, TEXT("Spawn Out"));

    // ActiveリストにAddして稼働中として管理する
    Pool->Active.Add(Enemy);
    return Enemy;
}

void UEnemyPoolSubSystem::Return(AEnemyBase* Enemy)
{
    if (!Enemy) { return; }

    UE_LOG(LogTemp, Warning, TEXT("Return In"));

    // どのPoolConfigのActorかを逆引きする
    UEnemyPoolConfig** DataPtr = EnemyToConfig.Find(Enemy);
    if (!DataPtr) { return; }

    FPrimaryAssetId Key = (*DataPtr)->GetPrimaryAssetId();
    FEnemyPool* Pool = Pools.Find(Key);
    if (!Pool) { return; }

    // ActiveからInactiveに移してDeactivateする
    Pool->Active.Remove(Enemy);
    Enemy->Deactivate();
    Pool->Inactive.Add(Enemy);

    UE_LOG(LogTemp, Warning, TEXT("Return Out"));
}

AEnemyBase* UEnemyPoolSubSystem::CreateNewEnemy(UEnemyPoolConfig* InData)
{
    FActorSpawnParameters Params;
    // コリジョン判定を無視して必ず生成する
    // Pool待機用の画面外に生成するため障害物はないが念のため設定する
    Params.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // Pool待機用の画面外座標
    // Deteactiveで非表示になるが座標も遠くに置いておく
    FVector HoldPos(0.f, 0.f, -10000.f);

    return GetWorld()->SpawnActor<AEnemyBase>(
        InData->EnemyClass, HoldPos, FRotator::ZeroRotator, Params);
}
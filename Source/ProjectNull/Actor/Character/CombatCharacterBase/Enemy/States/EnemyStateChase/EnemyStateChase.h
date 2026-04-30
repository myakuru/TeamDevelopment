#pragma once

/** 
* í«ê’State
*/

#include <ProjectNull/Utility/StateMachine/StateBase.h>

class AEnemyBase;

class FEnemyStateChase : public TStateBase<AEnemyBase>
{
protected:

	virtual void OnStart(AEnemyBase* Owner)override;
	virtual void OnUpdate(AEnemyBase* Owner, float DeltaTime)override;
	virtual void OnExit(AEnemyBase* Owner)override;

};

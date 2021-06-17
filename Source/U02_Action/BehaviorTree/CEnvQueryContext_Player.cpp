#include "CEnvQueryContext_Player.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Characters/CPlayer.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Components/CBehaviorComponent.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//감지기능으로 감지된 플레이어 얻어오기
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(ai->GetController());

	if (!!behavior->GetTargetPlayer())
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTargetPlayer());
}
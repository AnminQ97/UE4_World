#include "CBTTaskNode_Focus.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Focus::UCBTTaskNode_Focus()
{
	NodeName = "Focus";

}

EBTNodeResult::Type UCBTTaskNode_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	//No Target
	ACPlayer* target = behavior->GetTargetPlayer();

	if (target == NULL)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
	}
	else //플레이어 감지
	{
		//타겟이 있다면
		controller->SetFocus(target);
	}


	return EBTNodeResult::Succeeded;
}

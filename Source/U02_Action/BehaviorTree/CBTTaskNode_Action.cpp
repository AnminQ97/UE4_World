#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true; //tick사용을 위해 켜줘야댐
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	TotalTime = 0.0f;
	action->DoAction();

	return EBTNodeResult::InProgress ;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //controller
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //적개체
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	TotalTime += DeltaSeconds;

	if (state->IsIdleMode() && TotalTime > Delay) //아이들 상태이고 2초를 넘기면 다음으로 넘어감
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

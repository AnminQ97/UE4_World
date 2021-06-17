#include "CBTService_Wizard.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Wizard::UCBTService_Wizard()
{
	NodeName = "Wizard";
}

void UCBTService_Wizard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTree 가져오기
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //트리의 오너인 컨트롤러
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //컨트롤러의 오너인 적개체

	//State Component 가져오기
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	
	//Hitted 상태라면
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode(); //behavior에 Hitted 상태 넘기기

		return;
	}
	//No Target
	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
	{
		behavior->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	else //플레이어 감지
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}
	//타겟이 있다면
	controller->SetFocus(target);


	//적과 거리가 가까우면 도망
	float distance = ai->GetDistanceTo(target);
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetAvoidMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	//In Sight
	if (distance < controller->GetSightRadius())
	{
		behavior->SetActionMode();

		return;
	}

}

#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CPatrolComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee"; //이름변경
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTree 가져오기
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //트리의 오너인 컨트롤러
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //컨트롤러의 오너인 적개체
	
	//State Component 가져오기
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	//Patrol Component 가져오기
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	//Hitted 상태라면
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode(); //behavior에 Hitted 상태 넘기기

		return;
	}

	//감지된 타겟이 없다면
	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
	{
		//순찰
		if (patrol != NULL && patrol->IsValid())
		{
			behavior->SetPatrolMode();
			return;
		}
		
		behavior->SetWaitMode();
		return;
	}
	else
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}

	float distance = ai->GetDistanceTo(target); //타겟과의 거리
	
	//공격 범위 안에 있다면
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	//감지 범위 안에 있다면
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}

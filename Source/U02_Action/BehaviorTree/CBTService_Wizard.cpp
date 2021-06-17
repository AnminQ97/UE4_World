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

	//BehaviorTree ��������
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //Ʈ���� ������ ��Ʈ�ѷ�
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //��Ʈ�ѷ��� ������ ����ü

	//State Component ��������
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	
	//Hitted ���¶��
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode(); //behavior�� Hitted ���� �ѱ��

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
	else //�÷��̾� ����
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}
	//Ÿ���� �ִٸ�
	controller->SetFocus(target);


	//���� �Ÿ��� ������ ����
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

#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CPatrolComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee"; //�̸�����
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//BehaviorTree ��������
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //Ʈ���� ������ ��Ʈ�ѷ�
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //��Ʈ�ѷ��� ������ ����ü
	
	//State Component ��������
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);
	//Patrol Component ��������
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	//Hitted ���¶��
	if (state->IsHittedMode())
	{
		behavior->SetHittedMode(); //behavior�� Hitted ���� �ѱ��

		return;
	}

	//������ Ÿ���� ���ٸ�
	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == NULL)
	{
		//����
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

	float distance = ai->GetDistanceTo(target); //Ÿ�ٰ��� �Ÿ�
	
	//���� ���� �ȿ� �ִٸ�
	if (distance < controller->GetBehaviorRange())
	{
		behavior->SetActionMode();
		return;
	}

	//���� ���� �ȿ� �ִٸ�
	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();
		return;
	}
}

#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CPatrolComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";
	
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //controller
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //����ü
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	//�Լ� ȣ���� ���� ����, ���� ����
	FVector location;
	float acceptance;

	//������ ��ΰ� ���ٸ�
	if (patrol->GetMoveTo(location, acceptance) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress; // ����
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //controller
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //����ü
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	FVector location; //������
	float acceptance; //��Ȯ��
	patrol->GetMoveTo(location, acceptance);//location, acceptance�� �ֱ�

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptance, false);

	//�� �� ���°��̸� Failed
	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//���������� ���� ����
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		patrol->UpdateNextIndex(); //��Ʈ�� �ε��� ����
	}

}

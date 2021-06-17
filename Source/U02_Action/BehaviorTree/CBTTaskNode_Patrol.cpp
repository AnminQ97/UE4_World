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
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //적개체
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	//함수 호출을 위한 제물, 값은 없음
	FVector location;
	float acceptance;

	//세팅한 경로가 없다면
	if (patrol->GetMoveTo(location, acceptance) == false)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress; // 보류
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); //controller
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn()); //적개체
	UCPatrolComponent* patrol = CHelpers::GetComponent<UCPatrolComponent>(ai);

	FVector location; //목적지
	float acceptance; //정확도
	patrol->GetMoveTo(location, acceptance);//location, acceptance값 넣기

	EPathFollowingRequestResult::Type type = controller->MoveToLocation(location, acceptance, false);

	//갈 수 없는곳이면 Failed
	if (type == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	//목저지까지 가면 성공
	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		
		patrol->UpdateNextIndex(); //패트롤 인덱스 증가
	}

}

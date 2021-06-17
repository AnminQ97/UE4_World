#include "CBehaviorComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;

}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;

}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol;

}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;

}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;

}

UCBehaviorComponent::UCBehaviorComponent()
{

}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	//player ��ü ��������
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

FVector UCBehaviorComponent::GetWarpLocation()
{
	return Blackboard->GetValueAsVector(WarpLocationKey);
}

void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	//Type ����
	EBehaviorType type = GetType();
	Blackboard->SetValueAsEnum(BehaviorKey, uint8(InType));

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InType);
}

EBehaviorType UCBehaviorComponent::GetType()
{
	//������κ��� Ÿ�� ������ �뵵
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}

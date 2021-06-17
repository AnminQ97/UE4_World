#include "CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);

}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);

}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetSkillMode()
{
	ChangeType(EStateType::Skill);
}

void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType InPrvType = Type;
	Type = InNewType;

	
	if (OnStateTypeChaged.IsBound())
		OnStateTypeChaged.Broadcast(InPrvType, InNewType);
}


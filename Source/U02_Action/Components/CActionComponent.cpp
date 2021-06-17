#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "Actions/CSKill.h"
#include "Actions/CAttachment.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner());

	//ActionData::BeginPlay ����
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}
}

void UCActionComponent::SetUnarmedMode()
{
	//���⸦ ��ü�Ҷ� ��ϵ带 ��ġ�� ������ ������ ���Ⱑ �ִٸ� ����
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();
	//Unarmed����
	Datas[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetFireStormMode()
{
	SetMode(EActionType::FireStorm);
}

void UCActionComponent::SetIceBallMode()
{
	SetMode(EActionType::IceBall);
}

void UCActionComponent::OffAllCollisions()
{
	//�׼� �����Ͱ� �ִ���
	for (UCAction* data : Datas)
	{
		//�����Ͱ� ���̸� �н�
		if (!!data == false)
			continue;

		//�ߵ�ü�� ������ �н�
		if (!!data->GetAttachment() == false)
			continue;

		//�����Ͱ� �ֲ� �浹ü�� �ִ¾ֵ鸸 �ø��� ����
		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	//Data�� �ִٸ� 
	if (!!Datas[(int32)Type])
	{
		//���� Ÿ��
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->DoAction();
	}
}

void UCActionComponent::SkillAction(int32 key)
{
	CheckTrue(IsUnarmedMode());

	//Data�� �ִٸ� 
	if (!!Datas[(int32)Type])
	{
		//Ÿ��
		ACSkill* skill = Datas[(int32)Type]->GetSkill();
		if (!!skill)
			skill->SkillAction(key);
	}
}

void UCActionComponent::Dead()
{
	//������ ��� �ø��� ����
	OffAllCollisions();
}

void UCActionComponent::End_Dead()
{
	//������
	//��� Attachment, Equipment, DoAction �����
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i] && Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
		
		if (!!Datas[i] && Datas[i]->GetEquipment())
			Datas[i]->GetEquipment()->Destroy();

		if (!!Datas[i] && Datas[i]->GetDoAction())
			Datas[i]->GetDoAction()->Destroy();
	}

}

void UCActionComponent::AbortByDamage()
{
	CheckNull(Datas[(int32)Type]);
	CheckTrue(IsUnarmedMode());

	//���� �߿� ������� ���� ���, ������ ���Ͽ� �ٿ��ֱ�
	Datas[(int32)Type]->GetEquipment()->Begin_Equip();
	Datas[(int32)Type]->GetEquipment()->End_Equip();

	Datas[(int32)Type]->GetDoAction()->Abort();
}


void UCActionComponent::SetMode(EActionType InType)
{
	//������ ���� Ÿ���� ������ Unarmed
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if(IsUnarmedMode() == false)
	{
		//Unarmed�� �ƴ� �������� ���Ⱑ �ְ� �����͵� �����Ѵٸ� unequip
		if (!!Datas[(int32)Type])
			Datas[(int32)Type]->GetEquipment()->Unequip();

	}
	//���ο� ���� ����
	if (!!Datas[(int32)InType] && !!Datas[(int32)InType]->GetEquipment())
		Datas[(int32)InType]->GetEquipment()->Equip();


	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}

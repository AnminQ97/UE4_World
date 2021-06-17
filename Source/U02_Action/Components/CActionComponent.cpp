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

	//ActionData::BeginPlay 실행
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(character, &Datas[i]);
	}
}

void UCActionComponent::SetUnarmedMode()
{
	//무기를 교체할때 언암드를 거치기 때문에 기존에 무기가 있다면 해제
	if (!!Datas[(int32)Type])
		Datas[(int32)Type]->GetEquipment()->Unequip();
	//Unarmed장착
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
	//액션 데이터가 있는지
	for (UCAction* data : Datas)
	{
		//데이터가 널이면 패스
		if (!!data == false)
			continue;

		//중돌체가 없으면 패스
		if (!!data->GetAttachment() == false)
			continue;

		//데이터가 있꼬 충돌체가 있는애들만 컬리젼 끄기
		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	//Data가 있다면 
	if (!!Datas[(int32)Type])
	{
		//무기 타입
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if (!!action)
			action->DoAction();
	}
}

void UCActionComponent::SkillAction(int32 key)
{
	CheckTrue(IsUnarmedMode());

	//Data가 있다면 
	if (!!Datas[(int32)Type])
	{
		//타입
		ACSkill* skill = Datas[(int32)Type]->GetSkill();
		if (!!skill)
			skill->SkillAction(key);
	}
}

void UCActionComponent::Dead()
{
	//죽으면 모든 컬리젼 끄기
	OffAllCollisions();
}

void UCActionComponent::End_Dead()
{
	//죽으면
	//모든 Attachment, Equipment, DoAction 지우기
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

	//장착 중에 대미지를 받은 경우, 강제로 소켓에 붙여주기
	Datas[(int32)Type]->GetEquipment()->Begin_Equip();
	Datas[(int32)Type]->GetEquipment()->End_Equip();

	Datas[(int32)Type]->GetDoAction()->Abort();
}


void UCActionComponent::SetMode(EActionType InType)
{
	//기존과 같은 타입이 들어오면 Unarmed
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if(IsUnarmedMode() == false)
	{
		//Unarmed가 아닌 장착중인 무기가 있고 데이터도 존재한다면 unequip
		if (!!Datas[(int32)Type])
			Datas[(int32)Type]->GetEquipment()->Unequip();

	}
	//새로운 무기 장착
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

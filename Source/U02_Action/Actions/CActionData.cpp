#include "CActionData.h"
#include "Global.h"
#include "CAction.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "CSKill.h"
#include "GameFramework/Character.h"
#include "CDoAction.h"

void UCActionData::BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction)
{
	//그냥 항등행렬
	FTransform transform;

	ACAttachment* Attachment = NULL;
	if (!!AttachmentClass)
	{
		//spawn 예약 
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		//spawn 확정
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	ACEquipment* Equipment = NULL;
	if (!!EquipmentClass)
	{
		//지연 스폰
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true));
		Equipment->SetData(EquipmentData); //구조체 통째로 넣기
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter,"Equipment"));
		Equipment->SetColor(EquipmentColor);

		//스폰 예약
		//위에 있는 코드가 다 진행 되어야 스폰
		UGameplayStatics::FinishSpawningActor(Equipment, transform); 
		
		//장착할 형태가 있다면
		if (!!Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	//DoActionClass가 있다면
	ACDoAction* DoAction = NULL;
	if (!!DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		DoAction->SetActorLabel(GetLabelName(InOwnerCharacter, "DoAction"));
		DoAction->SetDatas(DoActionDatas);
		UGameplayStatics::FinishSpawningActor(DoAction, transform);

		if (!!Equipment)
		{
			//주소참조  
			DoAction->SetEquipped(Equipment->GetEquipped());
		}

		//무기가 있다면
		if (!!Attachment)
		{
			//무기에 따른 Overlap 함수
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);

		}
	}

	
	ACSkill* SKillData = NULL;
	if (!!SKillClass)
	{
		SKillData = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACSkill>(SKillClass, transform, InOwnerCharacter);
		SKillData->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		SKillData->SetActorLabel(GetLabelName(InOwnerCharacter, "SKill"));
		SKillData->SetDatas(SkillQ);
		SKillData->SetDatas(SkillW);
		SKillData->SetDatas(SkillE);
		UGameplayStatics::FinishSpawningActor(SKillData, transform);

		//무기가 있다면
		if (!!Attachment)
		{
			//무기에 따른 Overlap 함수
			Attachment->OnAttachmentBeginOverlap.AddDynamic(SKillData, &ACSkill::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(SKillData, &ACSkill::OnAttachmentEndOverlap);

		}
	}

	//결과들 CAction으로 깊은복사
	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = Attachment;
	(*OutAction)->Equipment = Equipment;
	(*OutAction)->DoAction = DoAction;
	(*OutAction)->Skill = SKillData;
	(*OutAction)->EquipmentColor = EquipmentColor;
}

FString UCActionData::GetLabelName(ACharacter* InOwnerCharacter, FString InName)
{
	FString name;
	name.Append(InOwnerCharacter->GetActorLabel());
	name.Append("_");
	name.Append(InName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_",L""));

	return name;
}

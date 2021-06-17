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
	//�׳� �׵����
	FTransform transform;

	ACAttachment* Attachment = NULL;
	if (!!AttachmentClass)
	{
		//spawn ���� 
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		//spawn Ȯ��
		UGameplayStatics::FinishSpawningActor(Attachment, transform);
	}

	ACEquipment* Equipment = NULL;
	if (!!EquipmentClass)
	{
		//���� ����
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true));
		Equipment->SetData(EquipmentData); //����ü ��°�� �ֱ�
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter,"Equipment"));
		Equipment->SetColor(EquipmentColor);

		//���� ����
		//���� �ִ� �ڵ尡 �� ���� �Ǿ�� ����
		UGameplayStatics::FinishSpawningActor(Equipment, transform); 
		
		//������ ���°� �ִٸ�
		if (!!Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	//DoActionClass�� �ִٸ�
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
			//�ּ�����  
			DoAction->SetEquipped(Equipment->GetEquipped());
		}

		//���Ⱑ �ִٸ�
		if (!!Attachment)
		{
			//���⿡ ���� Overlap �Լ�
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

		//���Ⱑ �ִٸ�
		if (!!Attachment)
		{
			//���⿡ ���� Overlap �Լ�
			Attachment->OnAttachmentBeginOverlap.AddDynamic(SKillData, &ACSkill::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(SKillData, &ACSkill::OnAttachmentEndOverlap);

		}
	}

	//����� CAction���� ��������
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

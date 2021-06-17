#include "CSkill_Action.h"
#include "Global.h"
#include "CThrow.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


void ACSkill_Action::BeginPlay()
{
	Super::BeginPlay();

}

void ACSkill_Action::SkillAction(int32 key)
{
	Key=key;

	CheckFalse(State->IsIdleMode());
	State->SetSkillMode();

	//��Ÿ��
	OwnerCharacter->PlayAnimMontage(Skilldatas[Key].AnimMontage, Skilldatas[Key].PlayRatio, Skilldatas[Key].StartSection);

	Skilldatas[Key].bCanMove ? Status->SetMove() : Status->SetStop();
}
void ACSkill_Action::Begin_SkillAction()
{


	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem"); //���� ����
	FVector forward = OwnerCharacter->GetActorForwardVector(); //�������

	//����Ʈ ���� ��ġ (�����ġ)
	FTransform transform = Skilldatas[Key].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(forward.Rotation()));

	//����																											  ��������
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Skilldatas[Key].SkillClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//�̺�Ʈ
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACSkill_Action::OnThrowBeginOverlap);

	//����Ȯ��
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

}
void ACSkill_Action::End_SkillAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACSkill_Action::Tick(float DeltaTime)
{

}

void ACSkill_Action::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e; //������ ����ȭ�� �� ���(���⼱ ������)							//����
	InHitResult.GetActor()->TakeDamage(Skilldatas[Key].Power, e, OwnerCharacter->GetController(), this);
}

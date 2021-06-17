#include "CDoAction_Throw.h"
#include "Global.h"
#include "CThrow.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay();


}

//���� Ŭ����
void ACDoAction_Throw::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	//������ ��Ÿ��
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem"); //���� ����
    //FRotator rotator = OwnerCharacter->GetControlRotation(); //���콺 ����
	FVector forward = OwnerCharacter->GetActorForwardVector(); //�������

	//����Ʈ ���� ��ġ (�����ġ)
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(forward.Rotation()));
	//transform.SetRotation(FQuat(rotator));

	//����																											  ��������
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);

	//����Ȯ��
	UGameplayStatics::FinishSpawningActor(throwObject, transform);
}

void ACDoAction_Throw::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
} 

void ACDoAction_Throw::Tick(float DeltaTime)
{
}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e; //������ ����ȭ�� �� ���(���⼱ ������)							//����
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

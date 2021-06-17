#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


//���ʹ�ư ������ ȣ��
void ACDoAction_Melee::DoAction()
{
	
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	//�޺��������� Ŭ��������
	if (bEnable == true)
	{
		bExist = true; 
		bEnable = false;
		return;
	}
	
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	//1Ÿ ���
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	
	//�޺���
	CheckFalse(bExist);
	bExist = false;

	//�������� ��Ÿ�� ����
	OwnerCharacter->StopAnimMontage();

	//�޺�Ÿ������
	Index++;
	if (Index > 3) Index = 3;
	//�޺� �ʰ� ����
	//FMath::Clamp<int32>(Index, 0, 3);

	//IndexŸ�� ��Ÿ�� �����			

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	//������ �����̰ų� �޺��� ���ҽ�
	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);
	Index = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOtherCharacter);
	
	CheckNull(InOtherCharacter);

	//�ٴ���Ʈ ����
	for (const ACharacter* other : HittedCharacter)
	{
		//�ѹ� �¾Ҵ��� üũ
		if (other == InOtherCharacter)
			return; //�¾����� ����
	}
	HittedCharacter.Add(InOtherCharacter);

	//Hit Stop
	float hitStop = Datas[Index].HitStop;
	//����  0�� �ƴ϶��
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "ResetGlobalTime", hitStop * 2e-2f, false);
	}

	//HitParticle
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation()); //�´� �� ��� ��ǥ
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;

	//ī�޶� ����ũ
	if (!!shake)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);

	//Damege�� �ִ� �Լ� 
	FDamageEvent e; //������ ����ȭ�� �� ���(���⼱ ������)							//����
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackerCauser);

}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackerCauser, InOtherCharacter);

	CheckNull(InOtherCharacter);

	//���� �˻縦 ���� �����
	HittedCharacter.Empty();
}

void ACDoAction_Melee::ResetGlobalTime()
{
	//�����ӵ���
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

}


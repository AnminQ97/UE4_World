#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"


//왼쪽버튼 누를때 호출
void ACDoAction_Melee::DoAction()
{
	
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);

	//콤보구간에서 클릭했을시
	if (bEnable == true)
	{
		bExist = true; 
		bEnable = false;
		return;
	}
	
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	//1타 재생
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	
	//콤보시
	CheckFalse(bExist);
	bExist = false;

	//진행중인 몽타주 끊기
	OwnerCharacter->StopAnimMontage();

	//콤보타수증가
	Index++;
	if (Index > 3) Index = 3;
	//콤보 초과 방지
	//FMath::Clamp<int32>(Index, 0, 3);

	//Index타수 몽타주 재생ㅇ			

	OwnerCharacter->PlayAnimMontage(Datas[Index].AnimMontage, Datas[Index].PlayRatio, Datas[Index].StartSection);
	Datas[Index].bCanMove ? Status->SetMove() : Status->SetStop();
	
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	//마지막 공격이거나 콤보를 안할시
	OwnerCharacter->StopAnimMontage(Datas[Index].AnimMontage);
	Index = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackerCauser, InOtherCharacter);
	
	CheckNull(InOtherCharacter);

	//다단히트 방지
	for (const ACharacter* other : HittedCharacter)
	{
		//한번 맞았는지 체크
		if (other == InOtherCharacter)
			return; //맞았으면 리턴
	}
	HittedCharacter.Add(InOtherCharacter);

	//Hit Stop
	float hitStop = Datas[Index].HitStop;
	//값이  0이 아니라면
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
		transform.AddToTranslation(InOtherCharacter->GetActorLocation()); //맞는 적 상대 좌표
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;

	//카메라 쉐이크
	if (!!shake)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);

	//Damege를 주는 함수 
	FDamageEvent e; //데미지 세분화할 때 사용(여기선 사용안함)							//무기
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, InAttacker->GetController(), InAttackerCauser);

}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackerCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InAttackerCauser, InOtherCharacter);

	CheckNull(InOtherCharacter);

	//다음 검사를 위해 비워줌
	HittedCharacter.Empty();
}

void ACDoAction_Melee::ResetGlobalTime()
{
	//원래속도로
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

}


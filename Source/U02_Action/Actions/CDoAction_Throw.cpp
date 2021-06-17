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

//공격 클릭시
void ACDoAction_Throw::DoAction()
{
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	//던지는 몽타주
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem"); //붙힐 소켓
    //FRotator rotator = OwnerCharacter->GetControlRotation(); //마우스 방향
	FVector forward = OwnerCharacter->GetActorForwardVector(); //전방방향

	//이펙트 보정 위치 (상대위치)
	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(forward.Rotation()));
	//transform.SetRotation(FQuat(rotator));

	//스폰																											  스폰조건
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);

	//스폰확정
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
	FDamageEvent e; //데미지 세분화할 때 사용(여기선 사용안함)							//무기
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

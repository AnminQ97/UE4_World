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

	//몽타주
	OwnerCharacter->PlayAnimMontage(Skilldatas[Key].AnimMontage, Skilldatas[Key].PlayRatio, Skilldatas[Key].StartSection);

	Skilldatas[Key].bCanMove ? Status->SetMove() : Status->SetStop();
}
void ACSkill_Action::Begin_SkillAction()
{


	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_ThrowItem"); //붙힐 소켓
	FVector forward = OwnerCharacter->GetActorForwardVector(); //전방방향

	//이펙트 보정 위치 (상대위치)
	FTransform transform = Skilldatas[Key].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(forward.Rotation()));

	//스폰																											  스폰조건
	ACThrow* throwObject = GetWorld()->SpawnActorDeferred<ACThrow>(Skilldatas[Key].SkillClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//이벤트
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACSkill_Action::OnThrowBeginOverlap);

	//스폰확정
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
	FDamageEvent e; //데미지 세분화할 때 사용(여기선 사용안함)							//무기
	InHitResult.GetActor()->TakeDamage(Skilldatas[Key].Power, e, OwnerCharacter->GetController(), this);
}

#include "CSkill.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACSkill::ACSkill()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACSkill::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
	
}

void ACSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


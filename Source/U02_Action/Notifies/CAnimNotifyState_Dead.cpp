#include "CAnimNotifyState_Dead.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"

FString UCAnimNotifyState_Dead::GetNotifyName_Implementation() const
{
	return "Dead";
}
void UCAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

}
void UCAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	//플레이어와 몹 다 사용 가능하게 IICharacter로 캐스트
	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();
}

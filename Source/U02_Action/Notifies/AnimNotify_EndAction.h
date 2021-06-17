#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EndAction.generated.h"

UCLASS()
class U02_ACTION_API UAnimNotify_EndAction : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	//노티파이 이름
	FString GetNotifyName_Implementation() const override;

	//노티파이 함수
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};

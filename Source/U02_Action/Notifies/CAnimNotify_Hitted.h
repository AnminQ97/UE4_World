#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Hitted.generated.h"

/**
 * 
 */
UCLASS()
class U02_ACTION_API UCAnimNotify_Hitted : public UAnimNotify
{
	GENERATED_BODY()

public:
	//��Ƽ���� �̸�
	FString GetNotifyName_Implementation() const override;

	//��Ƽ���� �Լ�
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

};

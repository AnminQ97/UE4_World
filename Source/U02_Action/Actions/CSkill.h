#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CSkill.generated.h"

UCLASS()
class U02_ACTION_API ACSkill : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetDatas(FSkillData InDatas) { Skilldatas.Add(InDatas); }
	


public:
	ACSkill();

public:
	virtual void SkillAction(int32 key) {}
	virtual void Begin_SkillAction() {}
	virtual void End_SkillAction() {}



public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackerCauser, class ACharacter* InOtherCharacter) {};

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackerCauser, class ACharacter* InOtherCharacter) {};


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;


protected:
	TArray<FSkillData> Skilldatas;
	

};

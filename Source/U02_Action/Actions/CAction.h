#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData //장착관련 필요한 정보들
{
	GENERATED_BODY()

public:
	//몽타주
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	//재생속도
	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	//섹션
	UPROPERTY(EditAnywhere)
		FName StartSection;

	//이동 가능 유무
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	//뒤돌기 가능 유무
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

//===============================================================

//각 타수마다 갖는 정보
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	//공격력
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	//경직
	UPROPERTY(EditAnywhere)
		float HitStop;

	//이펙트
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

};

//===============================================================

//스킬 정보
USTRUCT(BlueprintType)
struct FSkillData : public FEquipmentData
{
	GENERATED_BODY()

public:
	//공격력
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	//
	UPROPERTY(EditAnywhere)
		float HitStop;

	//이펙트
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> SkillClass;

};

UCLASS()
class U02_ACTION_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionData;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class ACSkill* GetSkill() { return Skill; }
	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }

private:
	//spawn에 클래스를 주면 나오는 객체를 저장할 변수
	//Spawndeferd를 통해 결과가 저장될 변수들
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction; //DoAction 자식을 저장받을 변수
	class ACSkill* Skill;
	FLinearColor EquipmentColor;
};

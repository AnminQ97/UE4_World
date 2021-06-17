#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData //�������� �ʿ��� ������
{
	GENERATED_BODY()

public:
	//��Ÿ��
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	//����ӵ�
	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	//����
	UPROPERTY(EditAnywhere)
		FName StartSection;

	//�̵� ���� ����
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	//�ڵ��� ���� ����
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
};

//===============================================================

//�� Ÿ������ ���� ����
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	//���ݷ�
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	//����
	UPROPERTY(EditAnywhere)
		float HitStop;

	//����Ʈ
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

//��ų ����
USTRUCT(BlueprintType)
struct FSkillData : public FEquipmentData
{
	GENERATED_BODY()

public:
	//���ݷ�
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	//
	UPROPERTY(EditAnywhere)
		float HitStop;

	//����Ʈ
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
	//spawn�� Ŭ������ �ָ� ������ ��ü�� ������ ����
	//Spawndeferd�� ���� ����� ����� ������
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction; //DoAction �ڽ��� ������� ����
	class ACSkill* Skill;
	FLinearColor EquipmentColor;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class U02_ACTION_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; } //�ּ� �ѱ�

public:	
	ACEquipment();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

public:
	//�������� ������ ����, Ű�� ������
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//��Ƽ���̿��� ȣ��
	//���� ���۽� (���� ��)
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//���� ����� (���� ��)
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

protected:
	//�������� ĳ����
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	//ĳ���� ���� (idle, Roll ��)
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	//ĳ���� ���� (bCanMove��)
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	bool bEquipped; //���� ���� bool

	FEquipmentData Data;
	FLinearColor Color;
};

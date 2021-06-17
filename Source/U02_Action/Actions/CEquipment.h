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
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; } //주소 넘김

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
	//블프에서 재정의 가능, 키를 누를시
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//노티파이에서 호출
	//장착 시작시 (소켓 등)
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//장착 종료시 (소켓 등)
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

protected:
	//장착중인 캐릭터
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	//캐릭터 상태 (idle, Roll 등)
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	//캐릭터 상태 (bCanMove등)
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	bool bEquipped; //워프 장착 bool

	FEquipmentData Data;
	FLinearColor Color;
};

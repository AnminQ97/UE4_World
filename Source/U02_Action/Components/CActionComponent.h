#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

//무기 모드
UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, FireStorm, IceBall, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	//DataAsset 배열
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* DataAssets[(int32)EActionType::Max]; //액션(무기 모드)의 경우의 수
public:

	//저장 받은 Data를 리턴
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }

	//상태 리턴 Get

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFireStormMode() { return Type == EActionType::FireStorm; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIceBallMode() { return Type == EActionType::IceBall; }


public:
	UCActionComponent();

	//상태 Set
	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();

	UFUNCTION(BlueprintCallable)
		void SetFistMode();

	UFUNCTION(BlueprintCallable)
		void SetOneHandMode();

	UFUNCTION(BlueprintCallable)		
		void SetTwoHandMode();
	
	UFUNCTION(BlueprintCallable)
		void SetWarpMode();
	
	UFUNCTION(BlueprintCallable)
		void SetFireStormMode();
	
	UFUNCTION(BlueprintCallable)
		void SetIceBallMode();

	void OffAllCollisions();

public:
	void DoAction();

	void SkillAction(int32 key);

	void Dead();
	void End_Dead(); 

	void AbortByDamage();

private:

	void SetMode(EActionType InType);
	//기존 무기를 다른 무기로 바꾸는 함수
	void ChangeType(EActionType InNewType);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;
	//ActionData에 Beginplay에 결과를 저장받을 변수

	UPROPERTY( )
		class UCAction* Datas[(int32)EActionType::Max];
		
};

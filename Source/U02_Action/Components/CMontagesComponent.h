#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

//데이터테이블에 컬럼으로 사용할 구조체
USTRUCT(BlueprintType) //블프사용
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY() //블프통신
		
public:
	UPROPERTY(EditAnywhere)
		EStateType Type; //상태

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage; //몽타주
	
	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f; //재생속도

	UPROPERTY(EditAnywhere)
		FName StartSection; //섹션
	
	UPROPERTY(EditAnywhere)
		bool bCanMove; //이동 가능 유무
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	//데이터테이블
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable; //데이터 테이블을 받을 변수

public:	
	UCMontagesComponent();

	void PlayRoll();
	void PlayBackStep();
	void PlayHitted();
	void PlayDead();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InStateType);

private:
	//데이터를 줄단위로 저장받을 구조체 배열
	FMontageData* Datas[(int32)EStateType::Max]; 
};

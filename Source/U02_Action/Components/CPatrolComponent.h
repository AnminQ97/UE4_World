#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		int32 Index;

	UPROPERTY(EditAnywhere)
		bool bReverse; //역방향

	UPROPERTY(EditAnywhere)
		float AcceptanceRadiuse = 50.0f; //포인트에 다가가는 정확도

public:
	FORCEINLINE bool IsValid() { return Path != NULL; } //순찰경로가 있는지

public:	
	UCPatrolComponent();
						 //위치           정확도
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadiuse); //다음 경로 위치
	void UpdateNextIndex(); //다음 포인트로 넘어가기

protected:
	virtual void BeginPlay() override;


};

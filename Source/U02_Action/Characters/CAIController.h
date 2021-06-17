#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U02_ACTION_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	//공격 감지 범위
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float AdjustCirclHeight = 50.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	//감지
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

public:
	ACAIController();
	virtual void Tick(float DeltaTime) override;

	//감지범위
	float GetSightRadius();

protected:
	virtual void BeginPlay() override;

	//어떤 폰에 빙의 됐는지 파라미터로 들어옴
	virtual void OnPossess(APawn* InPawn) override;
	//빙의 풀때
	virtual void OnUnPossess() override;


private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
private:
	class ACEnemy_AI* OwnerEnemy; //빙의가 된 적개체
	class UAISenseConfig_Sight* Sight; //시야감지

};

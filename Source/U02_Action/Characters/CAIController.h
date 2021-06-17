#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U02_ACTION_API ACAIController : public AAIController
{
	GENERATED_BODY()

private:
	//���� ���� ����
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;

	UPROPERTY(EditAnywhere)
		float AdjustCirclHeight = 50.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	//����
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }

public:
	ACAIController();
	virtual void Tick(float DeltaTime) override;

	//��������
	float GetSightRadius();

protected:
	virtual void BeginPlay() override;

	//� ���� ���� �ƴ��� �Ķ���ͷ� ����
	virtual void OnPossess(APawn* InPawn) override;
	//���� Ǯ��
	virtual void OnUnPossess() override;


private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
private:
	class ACEnemy_AI* OwnerEnemy; //���ǰ� �� ����ü
	class UAISenseConfig_Sight* Sight; //�þ߰���

};
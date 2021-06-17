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
		bool bReverse; //������

	UPROPERTY(EditAnywhere)
		float AcceptanceRadiuse = 50.0f; //����Ʈ�� �ٰ����� ��Ȯ��

public:
	FORCEINLINE bool IsValid() { return Path != NULL; } //������ΰ� �ִ���

public:	
	UCPatrolComponent();
						 //��ġ           ��Ȯ��
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadiuse); //���� ��� ��ġ
	void UpdateNextIndex(); //���� ����Ʈ�� �Ѿ��

protected:
	virtual void BeginPlay() override;


};

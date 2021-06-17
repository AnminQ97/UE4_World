#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCOptionComponent();

private:
	//���콺 ����
	UPROPERTY(EditAnywhere)
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditAnywhere)
		float VerticalLookRate = 45.0f;

public:
	//�ܺο��� ����ϱ� ���� Get�Լ�
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

protected:
	virtual void BeginPlay() override;

};

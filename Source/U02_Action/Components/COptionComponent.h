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
	//마우스 감도
	UPROPERTY(EditAnywhere)
		float HorizontalLookRate = 45.0f;

	UPROPERTY(EditAnywhere)
		float VerticalLookRate = 45.0f;

public:
	//외부에서 사용하기 위한 Get함수
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

protected:
	virtual void BeginPlay() override;

};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClickMoveController.generated.h"

 
UCLASS()
class U02_ACTION_API AClickMoveController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AClickMoveController();

private:
	void InputClickPressed();

	void InputClickReleased();

	virtual void SetupInputComponent() override;
	
	virtual void PlayerTick(float DeltaTime) override;

	void SetNewDestination(const FVector DestLocation);

	void MoveToMouseCursor();

	bool bClickMouse;


};

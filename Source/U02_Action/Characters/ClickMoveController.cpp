#include "ClickMoveController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AClickMoveController::AClickMoveController()
{
	bShowMouseCursor = true;
}

void AClickMoveController::InputClickPressed()
{
	bClickMouse = true;
}

void AClickMoveController::InputClickReleased()
{
	bClickMouse = false;
}

void AClickMoveController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("MoveClick", IE_Pressed, this, &AClickMoveController::InputClickPressed);
	InputComponent->BindAction("MoveClick", IE_Released, this, &AClickMoveController::InputClickReleased);


}

void AClickMoveController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bClickMouse)
	{
		MoveToMouseCursor();
	}

}

void AClickMoveController::MoveToMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		SetNewDestination(Hit.Location);
	}
	
}


void AClickMoveController::SetNewDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		if (Distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	
	}
}


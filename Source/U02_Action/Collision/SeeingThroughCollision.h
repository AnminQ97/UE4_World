#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeeingThroughCollision.generated.h"

UCLASS()
class U02_ACTION_API ASeeingThroughCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	ASeeingThroughCollision();

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor);


};

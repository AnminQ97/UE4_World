#include "SeeingThroughCollision.h"
#include "Components/BoxComponent.h"
#include "Global.h"
ASeeingThroughCollision::ASeeingThroughCollision()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box", RootComponent);

}

void ASeeingThroughCollision::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ASeeingThroughCollision::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ASeeingThroughCollision::ActorEndOverlap);

}

void ASeeingThroughCollision::ActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	CLog::Print(OverlappedActor->GetName());
}

void ASeeingThroughCollision::ActorEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}



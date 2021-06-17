#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnBeginOverlap, FHitResult, InHitResult);


UCLASS()
class U02_ACTION_API ACSpawn : public AActor
{
	GENERATED_BODY()
	
private:
	//파티클
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Explostion;

	//파티클 위치
	UPROPERTY(EditDefaultsOnly)
		FTransform ExplosionTransform;


	//충돌체
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	//투사체 파티클
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

public:	
	ACSpawn();

protected:
	virtual void BeginPlay() override;

private:
	//충돌함수
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FSpawnBeginOverlap OnSpawnBeginOverlap;
};

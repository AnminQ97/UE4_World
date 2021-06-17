#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBeginOverlap, FHitResult, InHitResult);

UCLASS()
class U02_ACTION_API ACThrow : public AActor
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

	//투사체컴포넌트
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	//생명
	UPROPERTY(EditAnywhere)
		int32 LifeSpan=10;

	UPROPERTY(EditAnywhere)
		bool bDestroy=false;

public:	
	ACThrow();

protected:
	virtual void BeginPlay() override;

private:
	//충돌함수
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;
};

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
	//��ƼŬ
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* Explostion;

	//��ƼŬ ��ġ
	UPROPERTY(EditDefaultsOnly)
		FTransform ExplosionTransform;

	//�浹ü
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	//����ü ��ƼŬ
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	//����ü������Ʈ
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	//����
	UPROPERTY(EditAnywhere)
		int32 LifeSpan=10;

	UPROPERTY(EditAnywhere)
		bool bDestroy=false;

public:	
	ACThrow();

protected:
	virtual void BeginPlay() override;

private:
	//�浹�Լ�
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;
};

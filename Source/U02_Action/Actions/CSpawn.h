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

public:	
	ACSpawn();

protected:
	virtual void BeginPlay() override;

private:
	//�浹�Լ�
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FSpawnBeginOverlap OnSpawnBeginOverlap;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

//BeginOverlap																				������						����								 ���� ���ϴ� ��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackerCauser, class ACharacter*, InOtherCharacter);
//EndOverlap
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackerCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class U02_ACTION_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

protected:
	//Mesh Attach
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);

	//�浹ü�� ���Ͽ� �ִ¿�
	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

public:
	ACAttachment();

	//�ø��� On/Off
	void OnCollision();
	void OffCollision();

protected:
	virtual void BeginPlay() override;

private:
	//Overlap Beign/End
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	
	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	//�ø��� �迭(��� ������ ����), Shape�� �ø��� �ֻ��� Ŭ����
	TArray<class UShapeComponent*> ShapeComponents;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class U02_ACTION_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8 TeamID = 0;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		float ZoomSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Zoom")
		FVector2D ZoomRange = FVector2D(0, 500);

//=====================================
//Widget
//=====================================
private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Player> UserWidgetClass;

	//클래스로부터 생성된 객체를 저장받을 변수
protected:
	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Player* UserWidget;

//=====================================
//Scene Component
//=====================================
private: 

	//스프링 암
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	//박스
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box;

	//카메라
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;
	

//=====================================
//Actor Component
//=====================================
private: 
	//Status //수치
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	//Option
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	//State //상태
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	//Montages 몽타주
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	//Action 관리 (공격)
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

public:
	ACPlayer();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//test
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);
	
	//캐릭터 공격방향 회전
	void CharacterRotation();

private:
	//속도제한 걷기/뛰기
	void OnWalk();
	void OffWalk();

private:
	//액션 이벤트
	void OnEvade();

private:
	void Begin_BackStep();
	void Begin_Roll();

public:
	void End_BackStep();
	void End_Roll();

private:
	void OnFist();
	void OnOneHand();
	void OnTwoHand();
	void OnIceBall();
	void OnWarp();

	//기본공격
	void OnDoAction();
	
	//Skill
	void OnSKillActionQ();
	void OnSKillActionW();
	void OnSKillActionE();

	void Hitted();
	void Dead();
	void End_Dead() override;

	void PickUp();
	
public:
	virtual void ChangeColor(FLinearColor InColor) override;

	UFUNCTION()
		void AddPlayHealth();

	//Inventroy
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Inventory")
		class UCInventoryComponent* Inventroy;

	//Inventory
	UFUNCTION(BlueprintCallable, Category = "Items")
		void UseItem(class ACItem* Item);

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
	class UMaterialInstanceDynamic* HelmatMaterial;

private:
	class AController* DamageInstigator; //데미지 입힌 대상

	bool Target = false;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SKillCool_1 = 5;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SKillCool_2 = 5;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SKillCool_3 = 5;


};

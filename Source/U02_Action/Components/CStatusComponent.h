#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSpeed : uint8
{
	Walk, Run, Sprint, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
		
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxMana = 100.0f;
private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ECharacterSpeed::Max] = { 200,400,600 };

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

	FORCEINLINE float GetMaxMana() { return MaxMana; }
	FORCEINLINE float GetMana() { return Mana; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ECharacterSpeed::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ECharacterSpeed::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ECharacterSpeed::Sprint];  }

	FORCEINLINE bool CanMove() { return bCanMove; }

public:
	UCStatusComponent();

	//체력증가
	void AddHealth(float InAmount);
	//체력감소
	void SubHealth(float InAmount);

	//Mp+
	void AddMana(float InAmount);
	//Mp-
	void SubMana(float InAmount);


	//bCanMove tru/false
	void SetMove();
	void SetStop();

	void SetSpeed(ECharacterSpeed InSpeed);

protected:
	virtual void BeginPlay() override;

private:
	float Health;
	float Mana;
	bool bCanMove = true;
		
};

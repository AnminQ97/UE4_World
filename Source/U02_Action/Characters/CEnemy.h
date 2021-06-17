#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"


UCLASS()
class U02_ACTION_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		bool bDrawName = false;

	UPROPERTY(EditDefaultsOnly)
		float LaunchValue = 100.0f;


private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;
private:
	//Status //��ġ
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	//Option
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	//State //����
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	//Montages ��Ÿ��
	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

public:
	ACEnemy();

	//�������� ���� �Լ�
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;

public:
	virtual void ChangeColor(FLinearColor InColor) override;

	UFUNCTION()
		void ResetColor();
private:
	//���º���
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevTypem, EStateType InNewType);

private:
	void Hitted();
	void Dead();
	void End_Dead() override;

private:
	class UMaterialInstanceDynamic* PantsMaterial;
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* HelmetMaterial;
	class UMaterialInstanceDynamic* SkritMaterial;

private:
	class AController* DamageInstigator; //���� �������� �����

};

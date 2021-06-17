#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontagesComponent.generated.h"

//���������̺� �÷����� ����� ����ü
USTRUCT(BlueprintType) //�������
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY() //�������
		
public:
	UPROPERTY(EditAnywhere)
		EStateType Type; //����

	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage; //��Ÿ��
	
	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f; //����ӵ�

	UPROPERTY(EditAnywhere)
		FName StartSection; //����
	
	UPROPERTY(EditAnywhere)
		bool bCanMove; //�̵� ���� ����
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	//���������̺�
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable; //������ ���̺��� ���� ����

public:	
	UCMontagesComponent();

	void PlayRoll();
	void PlayBackStep();
	void PlayHitted();
	void PlayDead();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage(EStateType InStateType);

private:
	//�����͸� �ٴ����� ������� ����ü �迭
	FMontageData* Datas[(int32)EStateType::Max]; 
};

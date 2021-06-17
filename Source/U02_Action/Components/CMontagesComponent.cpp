#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
UCMontagesComponent::UCMontagesComponent()
{
}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	//���� ���� ������ŭ ��
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		//�����Ϳ��� �о��
		for (FMontageData* data : datas)
		{
			//�̳� ��ȣ�� �迭��ȣ�� ���߱�
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}
	}

	
}


void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::BackStep);

}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}


//�Ķ���Ϳ� �´� ��Ÿ�� ���
void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	//playAnimMonatge �Լ��� ĳ���� �ȿ� �־ ���ʸ� ĳ���ͷ� ����
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}




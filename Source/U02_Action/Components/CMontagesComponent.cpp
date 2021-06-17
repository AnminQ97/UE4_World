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

	//상태 정의 개수만큼 돔
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		//데이터에서 읽어옴
		for (FMontageData* data : datas)
		{
			//이넘 번호랑 배열번호랑 맞추기
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


//파라미터에 맞는 몽타주 재생
void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	//playAnimMonatge 함수가 캐릭터 안에 있어서 오너를 캐릭터로 구함
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InStateType];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}




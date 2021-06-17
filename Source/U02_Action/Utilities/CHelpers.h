#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

class U02_ACTION_API CHelpers
{
public:
	//���� �������� �Լ�
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succed()");

		*OutObject = asset.Object;
	}

	//Begine ���� ������Ʈ����
	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj =Cast<T>(StaticLoadObject(T::StaticClass(), NULL,*InPath));
		verifyf(!!obj,L"!!obj"); // (!!) == (!=NULL)
		
		*OutObject = obj;
	}

	//������ Ŭ������ �������� �Լ�
	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{		
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(),L"asset.Succed()");

		*OutClass = asset.Class;
	}

	//�����ڿ��� ������Ʈ���� �Լ�
	template<typename T>	  //����, ������Ʈ ,������ �̸�, ��� ��Ÿġ ����
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL )
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
		
		//�θ� �ִ°��
		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		//�θ� ���� ��� RootComponent
		//set�Լ� ���ؼ� ������Ʈ �ֱ�
		InActor->SetRootComponent(*InComponent);
	}

	//Actor������Ʈ ���� create
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	//���͸� ������ ���;ȿ� �ִ� ������Ʈ�� ������
	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};


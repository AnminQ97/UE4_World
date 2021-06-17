#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

class U02_ACTION_API CHelpers
{
public:
	//에셋 가져오는 함수
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succed()");

		*OutObject = asset.Object;
	}

	//Begine 이후 오브젝트생성
	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj =Cast<T>(StaticLoadObject(T::StaticClass(), NULL,*InPath));
		verifyf(!!obj,L"!!obj"); // (!!) == (!=NULL)
		
		*OutObject = obj;
	}

	//에셋을 클래스로 가져오는 함수
	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{		
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(),L"asset.Succed()");

		*OutClass = asset.Class;
	}

	//생성자에서 오브젝트생성 함수
	template<typename T>	  //엑터, 컴포넌트 ,보여줄 이름, 어디에 어타치 할지
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL )
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
		
		//부모가 있는경우
		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		//부모가 없는 경우 RootComponent
		//set함수 통해서 컴포넌트 넣기
		InActor->SetRootComponent(*InComponent);
	}

	//Actor컴포넌트 전용 create
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	//엑터를 받으면 엑터안에 있는 컴포넌트를 가져옴
	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};


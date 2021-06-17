#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

UCLASS()
class U02_ACTION_API ACItem : public AActor
{
	GENERATED_BODY()
	
public:

	ACItem();



	//아이템이 있는 월드
	UPROPERTY(Transient)
		class UWorld* World;

	//아이템 사용 텍스트 (장착, 먹기 등)
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText UseActionText;
	
	//아이템 썸네일
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
		class UTexture2D* Thumbnail;

	//인벤토리에 보일 아이템 이름
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText ItemDisplayName;

	//아이템 설명
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		FText ItemDescription;

	//무게
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		float Weight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Item")
		int32 count = 0;

	//인벤토리 (위 정보들을 참조함)
	UPROPERTY()
		class UCInventoryComponent* OwningInventroy;

	//아이템에 따른 사용(action)
	virtual void Use(class ACPlayer* Character) {};

	//블프에서 정의 가능하게
	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class ACPlayer* Character);

protected:
	virtual void BeginPlay() override;

private:
	//충돌체
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

};

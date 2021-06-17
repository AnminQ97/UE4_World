#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

//인벤토리가 업데이트 될때 UI도 업데이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventroyUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U02_ACTION_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();

	virtual void BeginPlay() override;

	bool AddItem(class ACItem* Item);
	bool RemoveItem(class ACItem* Item);

	//에디터에서 추가한 아이템
	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class ACItem*> DefaultItems;

	//용량
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		int32 Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventroyUpdated OnInventroyUpdated;

	//현재 인벤토리
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Items")
		TArray<class ACItem*> Items;

	bool CheckItemName(FText name);
};

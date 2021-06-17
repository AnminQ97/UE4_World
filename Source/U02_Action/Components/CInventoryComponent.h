#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"

//�κ��丮�� ������Ʈ �ɶ� UI�� ������Ʈ
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

	//�����Ϳ��� �߰��� ������
	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<class ACItem*> DefaultItems;

	//�뷮
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
		int32 Capacity;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
		FOnInventroyUpdated OnInventroyUpdated;

	//���� �κ��丮
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Items")
		TArray<class ACItem*> Items;

	bool CheckItemName(FText name);
};

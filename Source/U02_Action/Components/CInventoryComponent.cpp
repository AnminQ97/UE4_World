#include "CInventoryComponent.h"
#include "Global.h"
#include "InventorySystem/CItem.h"

UCInventoryComponent::UCInventoryComponent()
{
	Capacity = 20;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/*for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}*/
	
}

//아이템 추가
bool UCInventoryComponent::AddItem(ACItem* Item)
{
	//용량 초과 거나 Null이면  리턴
	if (Items.Num() >= Capacity)
	{
		return false;
	}

	Item->OwningInventroy = this;
	Item->World = GetWorld();

	//중복이면
	if (CheckItemName(Item->ItemDisplayName))
	{
		
	}
	else//아니면
	{
		Items.Add(Item);
	}

	//UI 업데이트
	OnInventroyUpdated.Broadcast();

	return true;
}

//아이템 삭제
bool UCInventoryComponent::RemoveItem(ACItem* Item)
{
	if (Item)
	{
		if (Item->count > 0)
			Item->count--;
		else
		{
			Item->OwningInventroy = nullptr;
			Item->World = nullptr;
			Items.RemoveSingle(Item);
		}
		
		OnInventroyUpdated.Broadcast();
		return true;
	}

	return false;
}

bool UCInventoryComponent::CheckItemName(FText name)
{
	for (auto& Item : Items)
	{
		if (Item->ItemDisplayName.ToString() == name.ToString())
		{
			Item->count++;
			return true;
		}
		else
			Item->count++;
	}

	return false;
}

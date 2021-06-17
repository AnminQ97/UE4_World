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

//������ �߰�
bool UCInventoryComponent::AddItem(ACItem* Item)
{
	//�뷮 �ʰ� �ų� Null�̸�  ����
	if (Items.Num() >= Capacity)
	{
		return false;
	}

	Item->OwningInventroy = this;
	Item->World = GetWorld();

	//�ߺ��̸�
	if (CheckItemName(Item->ItemDisplayName))
	{
		
	}
	else//�ƴϸ�
	{
		Items.Add(Item);
	}

	//UI ������Ʈ
	OnInventroyUpdated.Broadcast();

	return true;
}

//������ ����
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

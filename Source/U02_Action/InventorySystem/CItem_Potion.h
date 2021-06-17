#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/CItem.h"
#include "CItem_Potion.generated.h"


UCLASS()
class U02_ACTION_API ACItem_Potion : public ACItem
{
	GENERATED_BODY()
	
public:
	virtual void Use(class ACPlayer* Character) override;
};

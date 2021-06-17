#include "CItem_Potion.h"
#include "CItem.h"


void ACItem_Potion::Use(class ACPlayer* Character)
{
	if (Character)
	{
		Character->AddPlayHealth();

		if (OwningInventroy)
		{
			OwningInventroy->RemoveItem(this);
		}
	}
}
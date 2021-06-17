#include "CItem.h"
#include "Global.h"
#include "Components/CInventoryComponent.h"
#include "Characters/CPlayer.h"
#include "Components/SphereComponent.h"

ACItem::ACItem()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");

	Weight = 1.0f;
	ItemDisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void ACItem::BeginPlay()
{
	Super::BeginPlay();

}
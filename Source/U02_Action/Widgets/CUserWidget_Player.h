#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Player.generated.h"

UCLASS()
class U02_ACTION_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
		void Update(float InHealth, float InMaxHealth, float InMana, float InMaxMana);

	UFUNCTION(BlueprintImplementableEvent)
		void SkillTime();
};

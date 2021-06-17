#pragma once

#include "CoreMinimal.h"
#include "Actions/CSkill.h"
#include "CSkill_Action.generated.h"

UCLASS()
class U02_ACTION_API ACSkill_Action : public ACSkill
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:

	virtual void SkillAction(int32 key) override;
	virtual void Begin_SkillAction() override;
	virtual void End_SkillAction() override;

	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult);
	int32 Key;

};

#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction.h"
#include "Actions/CSkill_Action.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}


void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());


	//근접은 콤보 / 마법은 스킬 스폰

	if (state->IsSkillMode())
	{
		action->GetCurrent()->GetSkill()->Begin_SkillAction();
	}
	else
	{
		action->GetCurrent()->GetDoAction()->Begin_DoAction();
	}


	
}

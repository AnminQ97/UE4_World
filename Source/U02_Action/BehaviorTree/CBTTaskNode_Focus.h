#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Focus.generated.h"

UCLASS()
class U02_ACTION_API UCBTTaskNode_Focus : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Focus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

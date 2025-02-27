#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ClearFocus.generated.h"

UCLASS()
class SPT_API UBTT_ClearFocus : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_ClearFocus();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

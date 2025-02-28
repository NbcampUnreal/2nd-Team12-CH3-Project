#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetRandomWaitTime.generated.h"

UCLASS()
class SPT_API UBTT_SetRandomWaitTime : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetRandomWaitTime();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
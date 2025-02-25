#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveAlongPatrolRoute.generated.h"

UCLASS()
class SPT_API UBTT_MoveAlongPatrolRoute : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveAlongPatrolRoute(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 Index = 0;
};

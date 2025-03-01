#include "BTT_SetRandomWaitTime.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_SetRandomWaitTime::UBTT_SetRandomWaitTime()
{
	bCreateNodeInstance = true;
	NodeName = "BTT_Set Random Wait Time";
}

EBTNodeResult::Type UBTT_SetRandomWaitTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    if (!BBC)
    {
        return EBTNodeResult::Failed;
    }

    float RandomTime = FMath::FRandRange(1.0f, 3.0f); // 1 ~ 3ÃÊ ·£´ý ´ë±â
    BBC->SetValueAsFloat(TEXT("RandomWaitTime"), RandomTime);

	return EBTNodeResult::Succeeded;
}

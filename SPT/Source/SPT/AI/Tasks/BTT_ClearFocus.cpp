#include "BTT_ClearFocus.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTT_ClearFocus::UBTT_ClearFocus()
{
    bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Clear Focus");
}

EBTNodeResult::Type UBTT_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (AIC)
    {
        AIC->ClearFocus(EAIFocusPriority::Gameplay);
    }
    return EBTNodeResult::Succeeded;
}
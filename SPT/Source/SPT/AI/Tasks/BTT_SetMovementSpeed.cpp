#include "BTT_SetMovementSpeed.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"

UBTT_SetMovementSpeed::UBTT_SetMovementSpeed()
{
    NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        ASPTEnemyCharacter* EnemyCharacter = Cast<ASPTEnemyCharacter>(AIController->GetPawn());
        if (EnemyCharacter)
        {
            EnemyCharacter->SetMovementSpeed(MovementSpeed);
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}

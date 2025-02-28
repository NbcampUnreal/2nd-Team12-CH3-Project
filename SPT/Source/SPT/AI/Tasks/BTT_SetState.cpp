#include "BTT_SetState.h"
#include "SPTEnemyController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetState::UBTT_SetState()
{
    NodeName = TEXT("Set AI State");
}

EBTNodeResult::Type UBTT_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI 컨트롤러 가져오기
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is NULL!"));
        return EBTNodeResult::Failed;
    }

    // `SPTEnemyController`로 캐스팅
    ASPTEnemyController* EnemyController = Cast<ASPTEnemyController>(AIC);
    if (!EnemyController)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast AIController to ASPTEnemyController!"));
        return EBTNodeResult::Failed;
    }

    // `NewState` 값을 기반으로 상태 변경
    if (NewState == 0) // 0 = Passive
    {
        EnemyController->SetStateAsPassive();
    }
    else
    {
        EnemyController->SetStateAsAttacking(nullptr); // 공격 대상 없음
    }

    UE_LOG(LogTemp, Warning, TEXT("AI State changed to: %d"), NewState);
    return EBTNodeResult::Succeeded;
}

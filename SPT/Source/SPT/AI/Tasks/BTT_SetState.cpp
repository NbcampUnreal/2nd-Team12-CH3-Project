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
    // AI ��Ʈ�ѷ� ��������
    AAIController* AIC = OwnerComp.GetAIOwner();
    if (!AIC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is NULL!"));
        return EBTNodeResult::Failed;
    }

    // `SPTEnemyController`�� ĳ����
    ASPTEnemyController* EnemyController = Cast<ASPTEnemyController>(AIC);
    if (!EnemyController)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast AIController to ASPTEnemyController!"));
        return EBTNodeResult::Failed;
    }

    // `NewState` ���� ������� ���� ����
    if (NewState == 0) // 0 = Passive
    {
        EnemyController->SetStateAsPassive();
    }
    else
    {
        EnemyController->SetStateAsAttacking(nullptr); // ���� ��� ����
    }

    UE_LOG(LogTemp, Warning, TEXT("AI State changed to: %d"), NewState);
    return EBTNodeResult::Succeeded;
}

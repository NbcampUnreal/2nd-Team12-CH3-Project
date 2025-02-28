#include "BTT_MoveAlongPatrolRoute.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRoute.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Navigation/PathFollowingComponent.h"

UBTT_MoveAlongPatrolRoute::UBTT_MoveAlongPatrolRoute(const FObjectInitializer& ObjectInitializer)
{
    bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Move Along Patrol Route");
}

EBTNodeResult::Type UBTT_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    OwnerCompRef = &OwnerComp;

    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    AAIController* EnemyController = OwnerComp.GetAIOwner();

    if (!EnemyController || !BBC)
    {
        return EBTNodeResult::Failed;
    }

    // AI ĳ���� ��������
    ASPTEnemyCharacter* EnemyCharacter = Cast<ASPTEnemyCharacter>(EnemyController->GetPawn());
    if (!EnemyCharacter || !EnemyCharacter->PatrolRoute)
    {
        return EBTNodeResult::Failed;
    }

    // PatrolRoute���� ���� ���� ��������
    FVector NextPatrolPoint = EnemyCharacter->PatrolRoute->GetNextPatrolPoint(EnemyCharacter);

    // ������ �� ����
    BBC->SetValueAsVector(FName("MoveToLocation"), NextPatrolPoint);

    // AI �̵� ��� ����
    EPathFollowingRequestResult::Type MoveResult = EnemyController->MoveToLocation(NextPatrolPoint);

    // �̵� ��û�� �����ϸ� ��� ����
    if (MoveResult != EPathFollowingRequestResult::RequestSuccessful)
    {
        return EBTNodeResult::Failed;
    }

    EnemyController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_MoveAlongPatrolRoute::OnMoveCompleted);

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_MoveAlongPatrolRoute::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* EnemyController = OwnerComp.GetAIOwner();
    if (!EnemyController)
    {
        return EBTNodeResult::Failed;
    }
    else
    {
        EnemyController->StopMovement();
    }


    return EBTNodeResult::Aborted;
}

void UBTT_MoveAlongPatrolRoute::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (Result.Code == EPathFollowingResult::Success && OwnerCompRef)
    {
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}
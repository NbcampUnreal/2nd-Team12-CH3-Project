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

    // AI 캐릭터 가져오기
    ASPTEnemyCharacter* EnemyCharacter = Cast<ASPTEnemyCharacter>(EnemyController->GetPawn());
    if (!EnemyCharacter || !EnemyCharacter->PatrolRoute)
    {
        return EBTNodeResult::Failed;
    }

    // PatrolRoute에서 다음 지점 가져오기
    FVector NextPatrolPoint = EnemyCharacter->PatrolRoute->GetNextPatrolPoint(EnemyCharacter);

    // 블랙보드 값 설정
    BBC->SetValueAsVector(FName("MoveToLocation"), NextPatrolPoint);

    // AI 이동 명령 실행
    EPathFollowingRequestResult::Type MoveResult = EnemyController->MoveToLocation(NextPatrolPoint);

    // 이동 요청이 실패하면 즉시 종료
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
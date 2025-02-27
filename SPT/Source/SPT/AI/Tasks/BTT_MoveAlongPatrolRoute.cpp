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

void UBTT_MoveAlongPatrolRoute::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (Result.Code == EPathFollowingResult::Success && OwnerCompRef)
    {
        FinishLatentTask(*OwnerCompRef, EBTNodeResult::Succeeded);
    }
}



/*EBTNodeResult::Type UBTT_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    AAIController* EnemyController = OwnerComp.GetAIOwner();

    if (!EnemyController || !BBC)
    {
        return EBTNodeResult::Failed;
    }

    ASPTEnemyCharacter* EnemyCharacter = Cast<ASPTEnemyCharacter>(EnemyController->GetPawn());
    if (!EnemyCharacter || !EnemyCharacter->PatrolRoute)
    {
        return EBTNodeResult::Failed;
    }

    APatrolRoute* PathRef = EnemyCharacter->PatrolRoute;
    USplineComponent* SplinePath = PathRef->Path;

    // 스플라인이 제대로 설정되었는지 확인
    if (!SplinePath || SplinePath->GetNumberOfSplinePoints() == 0)
    {
        return EBTNodeResult::Succeeded;
    }

    // 스플라인 포인트 가져오기
    FVector NextPatrolPoint = SplinePath->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
    // 적 이름 가져오기
    FString EnemyName = EnemyCharacter->GetName();
    // 스플라인 포인트 로그 출력
    UE_LOG(LogTemp, Warning, TEXT("[%s] Next Patrol Point: %s"), *EnemyName, *NextPatrolPoint.ToString());

    BBC->SetValueAsVector(FName("MoveToLocation"), NextPatrolPoint);
    UE_LOG(LogTemp, Warning, TEXT("[%s] Blackboard Location Set To: %s"), *EnemyName, *BBC->GetValueAsVector(FName("MoveToLocation")).ToString());

    // 인덱스 순환 (무한 루프 순찰)
    Index = (Index + 1) % SplinePath->GetNumberOfSplinePoints();

    return EBTNodeResult::Succeeded;
}*/
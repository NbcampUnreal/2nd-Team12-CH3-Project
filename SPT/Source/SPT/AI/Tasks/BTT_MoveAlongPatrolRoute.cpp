#include "BTT_MoveAlongPatrolRoute.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PatrolRoute.h"

UBTT_MoveAlongPatrolRoute::UBTT_MoveAlongPatrolRoute(const FObjectInitializer& ObjectInitializer)
{
    bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Move Along Patrol Route");
}

EBTNodeResult::Type UBTT_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    // ���ö����� ����� �����Ǿ����� Ȯ��
    if (!SplinePath || SplinePath->GetNumberOfSplinePoints() == 0)
    {
        return EBTNodeResult::Succeeded;
    }

    // ���ö��� ����Ʈ ��������
    FVector NextPatrolPoint = SplinePath->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
    // �� �̸� ��������
    FString EnemyName = EnemyCharacter->GetName();
    // ���ö��� ����Ʈ �α� ���
    UE_LOG(LogTemp, Warning, TEXT("[%s] Next Patrol Point: %s"), *EnemyName, *NextPatrolPoint.ToString());

    BBC->SetValueAsVector(FName("MoveToLocation"), NextPatrolPoint);
    UE_LOG(LogTemp, Warning, TEXT("[%s] Blackboard Location Set To: %s"), *EnemyName, *BBC->GetValueAsVector(FName("MoveToLocation")).ToString());

    // �ε��� ��ȯ (���� ���� ����)
    Index = (Index + 1) % SplinePath->GetNumberOfSplinePoints();

    return EBTNodeResult::Succeeded;
}
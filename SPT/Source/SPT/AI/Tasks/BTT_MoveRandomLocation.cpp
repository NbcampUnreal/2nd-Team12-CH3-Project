#include "BTT_MoveRandomLocation.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "AISpawnVolume.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"

UBTT_MoveRandomLocation::UBTT_MoveRandomLocation()
{
    bCreateNodeInstance = true;
    NodeName = "Move To Random Location Within Spawn Volume";
}

EBTNodeResult::Type UBTT_MoveRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ASPTEnemyController* AIC = Cast<ASPTEnemyController>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

    if (!AIC || !BBC || !NavSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController, BlackboardComponent, or NavSystem is NULL!"));
        return EBTNodeResult::Failed;
    }

    APawn* ControlledPawn = AIC->GetPawn();
    if (!ControlledPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("ControlledPawn is NULL!"));
        return EBTNodeResult::Failed;
    }

    // AI가 속한 스폰 볼륨 찾기
    AAISpawnVolume* SpawnVolume = nullptr;
    for (TActorIterator<AAISpawnVolume> It(AIC->GetWorld()); It; ++It)
    {
        SpawnVolume = *It;
        break;  // 첫 번째 발견된 스폰 볼륨 사용
    }

    if (!SpawnVolume)
    {
        UE_LOG(LogTemp, Error, TEXT("No valid SpawnVolume found!"));
        return EBTNodeResult::Failed;
    }

    FVector Destination = GetRandomPointInSpawnVolume(SpawnVolume);

    if (!Destination.IsZero())
    {
        AIC->MoveToLocation(Destination);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

FVector UBTT_MoveRandomLocation::GetRandomPointInSpawnVolume(AAISpawnVolume* SpawnVolume)
{
    if (!SpawnVolume) return FVector::ZeroVector;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    if (!NavSystem) return FVector::ZeroVector;

    FNavLocation NavLocation;
    FVector RandomPoint;
    FVector BoxExtent = SpawnVolume->GetComponentsBoundingBox().GetExtent();
    FVector BoxOrigin = SpawnVolume->GetActorLocation();

    // NavMesh 위에서 유효한 위치 찾기 (최대 10회 시도)
    for (int i = 0; i < 10; i++)
    {
        RandomPoint = BoxOrigin + FVector(
            FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
            FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
            0.0f  // Z값 유지
        );

        if (NavSystem->ProjectPointToNavigation(RandomPoint, NavLocation))
        {
            // 생성된 위치가 `SpawnVolume` 내부에 있는지 확인
            if (SpawnVolume->GetComponentsBoundingBox().IsInsideOrOn(NavLocation.Location))
            {
                return NavLocation.Location;
            }
        }
    }

    return FVector::ZeroVector;  // 유효한 위치가 없으면 실패
}

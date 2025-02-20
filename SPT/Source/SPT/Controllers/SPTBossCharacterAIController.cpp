#include "SPTBossCharacterAIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

void ASPTBossCharacterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void ASPTBossCharacterAIController::BeginPlay()
{
    Super::BeginPlay();
}

void ASPTBossCharacterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    if (Result.Code == EPathFollowingResult::Success)
    {
    }
}
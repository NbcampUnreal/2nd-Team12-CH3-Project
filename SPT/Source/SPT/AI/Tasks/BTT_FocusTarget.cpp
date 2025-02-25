#include "BTT_FocusTarget.h"
#include "SPTEnemyCharacter.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_FocusTarget::UBTT_FocusTarget()
{
	bCreateNodeInstance = true;
    NodeName = TEXT("BTT_Focus Target");
    // 실제로 Blackboard에서 사용될 기본 Key 이름 지정
    FocusTargetKey.SelectedKeyName = FName("AttackTarget");
    FocusTargetKey.AddObjectFilter(
        this,
        GET_MEMBER_NAME_CHECKED(UBTT_FocusTarget, FocusTargetKey),
        AActor::StaticClass()
    );
}


EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIC = OwnerComp.GetAIOwner();
    BBC = OwnerComp.GetBlackboardComponent();
    ACharacter* FocusTarget = Cast<ACharacter>(BBC->GetValueAsObject(FocusTargetKey.SelectedKeyName));

    if (!AIC || !BBC)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController or BlackboardComponent is NULL!"));
        return EBTNodeResult::Failed;
    }
    
    if (FocusTarget)
    {
        AIC->SetFocus(FocusTarget);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

// #include "Kismet/GameplayStatics.h"
// UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
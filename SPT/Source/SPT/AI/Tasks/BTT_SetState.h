#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetState.generated.h"

UCLASS()
class SPT_API UBTT_SetState : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTT_SetState();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    // 변경할 AI 상태 (0 = Passive, 1 = Attacking)
    UPROPERTY(EditAnywhere, Category = "AI")
    uint8 NewState;
};

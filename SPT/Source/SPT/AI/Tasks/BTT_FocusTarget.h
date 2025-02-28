#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FocusTarget.generated.h"

class ASPTEnemyCharacter;

UCLASS()
class SPT_API UBTT_FocusTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_FocusTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FBlackboardKeySelector FocusTargetKey;

	UPROPERTY()
	UBlackboardComponent* BBC = nullptr;
};
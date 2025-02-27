#pragma once

#include "CoreMinimal.h"
#include "SPTEnemyCharacter.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetMovementSpeed.generated.h"

UCLASS()
class SPT_API UBTT_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_SetMovementSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EMovementSpeed MovementSpeed;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

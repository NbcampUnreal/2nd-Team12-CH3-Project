#pragma once

#include "CoreMinimal.h"
#include "SPTEnemyController.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MoveAlongPatrolRoute.generated.h"

UCLASS()
class SPT_API UBTT_MoveAlongPatrolRoute : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_MoveAlongPatrolRoute(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	UBehaviorTreeComponent* OwnerCompRef = nullptr;
};

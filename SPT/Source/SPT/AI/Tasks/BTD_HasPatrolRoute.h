#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_HasPatrolRoute.generated.h"

UCLASS()
class SPT_API UBTD_HasPatrolRoute : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_HasPatrolRoute();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};

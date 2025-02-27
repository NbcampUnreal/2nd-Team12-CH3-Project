#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsEquippedWeapon.generated.h"

UCLASS()
class SPT_API UBTD_IsEquippedWeapon : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_IsEquippedWeapon();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};

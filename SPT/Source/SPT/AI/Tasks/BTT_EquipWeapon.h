#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_EquipWeapon.generated.h"

class ASPTEnemyCharacter;

UCLASS()
class SPT_API UBTT_EquipWeapon : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_EquipWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
	ASPTEnemyCharacter* EnemyCharacterRef = nullptr;
};

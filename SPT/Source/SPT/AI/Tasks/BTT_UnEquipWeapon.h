#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UnEquipWeapon.generated.h"

class ASPTEnemyCharacter;

UCLASS()
class SPT_API UBTT_UnEquipWeapon : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_UnEquipWeapon();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
	ASPTEnemyCharacter* EnemyCharacterRef = nullptr;
};

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_DefaultAttack.generated.h"

class ASPTEnemyCharacter;

UCLASS()
class SPT_API UBTT_DefaultAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTT_DefaultAttack();

	// 공격 종료 시 호출되는 함수
	UFUNCTION()
	void OnAttackFinished();

	UPROPERTY()
	ASPTEnemyCharacter* EnemyCharacterRef = nullptr;
	UPROPERTY()
	UBehaviorTreeComponent* OwnerCompRef = nullptr;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
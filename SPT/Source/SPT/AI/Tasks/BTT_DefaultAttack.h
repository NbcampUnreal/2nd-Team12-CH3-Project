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

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// ���� ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnAttackFinished();

	UPROPERTY()
	ASPTEnemyCharacter* EnemyCharacterRef = nullptr;
	UPROPERTY()
	UBehaviorTreeComponent* StoredOwnerComp = nullptr;
};
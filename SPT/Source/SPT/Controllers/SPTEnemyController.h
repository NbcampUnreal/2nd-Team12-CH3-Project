#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SPTEnemyController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class FTimerManager;

UCLASS()
class SPT_API ASPTEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ASPTEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// �÷��̾� ĳ���͸� Ÿ������ �����ϴ� �ʱ�ȭ �Լ�
	void InitializeBlackboard();

	// �����̿� Ÿ�̸� �ڵ鷯
	FTimerHandle BlackboardInitTimerHandle;

public:
	// �����̺�� Ʈ�� ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BBC;
};

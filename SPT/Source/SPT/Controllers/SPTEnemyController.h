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

	// 플레이어 캐릭터를 타겟으로 설정하는 초기화 함수
	void InitializeBlackboard();

	// 딜레이용 타이머 핸들러
	FTimerHandle BlackboardInitTimerHandle;

public:
	// 비헤이비어 트리 에셋
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// 블랙보드 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BBC;
};

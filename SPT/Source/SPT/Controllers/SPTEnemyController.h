#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "SPTEnemyController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardData;

UENUM(BlueprintType)
enum class EAISenseType : uint8
{
	None     UMETA(DisplayName = "None"),
	Sight    UMETA(DisplayName = "Sight"),
	Hearing  UMETA(DisplayName = "Hearing"),
	Damage   UMETA(DisplayName = "Damage")
};

UCLASS()
class SPT_API ASPTEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ASPTEnemyController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:

	UFUNCTION(BlueprintCallable)
	void SetStateAsPassive();

	UFUNCTION(BlueprintCallable)
	void SetStateAsAttacking(AActor* AttackTarget);

	UFUNCTION(BlueprintCallable)
	void SetStateAsInvestigating(FVector Location);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool CanSenseActor(AActor* Actor, EAISenseType Sense, FAIStimulus& OutStimulus) const;

	UFUNCTION(BlueprintCallable, Category = "AI")
	int32 GetCurrentState() const;

	// 비헤이비어 트리 에셋
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// 블랙보드 데이터 (UBlackboardData)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

	// 블랙보드 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BBC;

private:

	// AI 인식 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;

	UFUNCTION(BlueprintCallable)
	void HandleSensedSight(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void HandleSensedSound(FVector Location);

	UFUNCTION(BlueprintCallable)
	void HandleSensedDamage(AActor* Actor);

	TArray<AActor*> KnownSeenActors;   // 시야로 감지한 액터 목록
	TArray<AActor*> KnownDamagedActors; // 피해를 준 액터 목록

	UFUNCTION(BlueprintCallable)
	void CheckIfForgottenSeenActor();

	UFUNCTION(BlueprintCallable)
	void CheckIfForgottenDamagedActor();

	UFUNCTION(BlueprintCallable)
	void HandleForgotActor(AActor* Actor);


	FTimerHandle ForgottenActorTimerHandle;       // 시야 감지한 액터를 잊는 타이머
	FTimerHandle ForgottenDamagedActorTimerHandle; // 피해 감지한 액터를 잊는 타이머

};

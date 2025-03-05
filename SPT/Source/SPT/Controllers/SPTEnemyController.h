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

	// �����̺�� Ʈ�� ����
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// ������ ������ (UBlackboardData)
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;

	// ������ ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BBC;

private:

	// AI �ν� ������Ʈ
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

	TArray<AActor*> KnownSeenActors;   // �þ߷� ������ ���� ���
	TArray<AActor*> KnownDamagedActors; // ���ظ� �� ���� ���

	UFUNCTION(BlueprintCallable)
	void CheckIfForgottenSeenActor();

	UFUNCTION(BlueprintCallable)
	void CheckIfForgottenDamagedActor();

	UFUNCTION(BlueprintCallable)
	void HandleForgotActor(AActor* Actor);


	FTimerHandle ForgottenActorTimerHandle;       // �þ� ������ ���͸� �ش� Ÿ�̸�
	FTimerHandle ForgottenDamagedActorTimerHandle; // ���� ������ ���͸� �ش� Ÿ�̸�

};

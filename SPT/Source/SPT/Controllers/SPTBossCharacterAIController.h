#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SPTBossCharacterAIController.generated.h"


UCLASS()
class SPT_API ASPTBossCharacterAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	APawn* PlayerPawn;

	UFUNCTION(BlueprintCallable, Category = "AI")
	APawn* GetTargetPlayer() const;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PatrolRoute.generated.h"

UCLASS(Blueprintable)
class SPT_API APatrolRoute : public AActor
{
    GENERATED_BODY()

public:
    APatrolRoute();

    UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "AI")
    USplineComponent* Path;  // ¼øÂû °æ·Î

protected:
    virtual void BeginPlay() override;
};
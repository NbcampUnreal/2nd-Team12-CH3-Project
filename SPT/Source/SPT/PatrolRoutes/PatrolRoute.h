#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PatrolRoute.generated.h"

UCLASS()
class SPT_API APatrolRoute : public AActor
{
    GENERATED_BODY()

public:
    APatrolRoute();

    UFUNCTION(BlueprintCallable, Category = "Patrol")
    void IncrementPatrolRoute();

    UFUNCTION(BlueprintCallable, Category = "Patrol")
    FVector GetNextPatrolPoint() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USplineComponent* PatrolSpline;

private:
    int32 PatrolIndex;
    int32 Direction;
};

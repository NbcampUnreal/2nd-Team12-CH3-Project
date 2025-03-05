// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecordBoard.generated.h"

UCLASS()
class SPT_API ARecordBoard : public AActor
{
    GENERATED_BODY()

public:
    ARecordBoard();

protected:
    virtual void BeginPlay() override;

public:
    void RegisterHit(FVector TargetHitLocation);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* MeshComp;

private:
    FVector ConvertToRecordBoardSpace(FVector TargetHitRatio);

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<AActor> MarkerBlueprint;

    UPROPERTY(EditAnywhere, Category = "Setup")    
    TArray<AActor*> MarkerList;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitTarget.generated.h"

UCLASS()
class SPT_API AHitTarget : public AActor
{
	GENERATED_BODY()
	
public:
    AHitTarget();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* TargetMeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    UStaticMesh* CubeMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UStaticMeshComponent* BoardMeshComp;



    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION()
    void RecordHit(FVector HitLocation);

    UFUNCTION()
    void DrawHitMarker(FVector HitLocation);

    // 표적지 맞은 위치를 다른 표적지로 옮기는 함수
    void MoveToOtherTarget(FVector HitLocation);
};


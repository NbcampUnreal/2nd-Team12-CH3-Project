// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSpawnerTrigger.generated.h"

UCLASS()
class SPT_API ABossSpawnerTrigger : public AActor
{
	GENERATED_BODY()
	
public:
    ABossSpawnerTrigger();

    UPROPERTY(EditAnywhere, Category = "Boss")
    TSubclassOf<AActor> BossClass;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    FVector SpawnLocation;

    UPROPERTY(EditAnywhere, Category = "Objects")
    TArray<AActor*> TargetObjects; // 감시할 오브젝트 목록

    UPROPERTY(EditAnywhere, Category = "Rotation")
    AActor* RotatingObject; // 회전시킬 오브젝트

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationAngle = 90.0f; // 회전할 각도 (기본 90도)

    // 회전을 되돌리기 위한 시간
    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationResetDelay = 15.0f;  // 몇 초 후에 원래 회전으로 되돌릴지 설정

    // 타이머 핸들
    FTimerHandle RotationResetTimerHandle;

    virtual void BeginPlay() override;

    int HitCount = 0;

    UFUNCTION()
    void OnObjectHit();  // TriggerObject가 맞았을 때 실행

    // 회전 값을 원래대로 되돌리는 함수
    UFUNCTION()
    void ResetRotation();
};

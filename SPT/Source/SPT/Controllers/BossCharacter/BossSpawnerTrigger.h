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
    TArray<AActor*> TargetObjects; // ������ ������Ʈ ���

    UPROPERTY(EditAnywhere, Category = "Rotation")
    AActor* RotatingObject; // ȸ����ų ������Ʈ

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationAngle = 90.0f; // ȸ���� ���� (�⺻ 90��)

    // ȸ���� �ǵ����� ���� �ð�
    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RotationResetDelay = 15.0f;  // �� �� �Ŀ� ���� ȸ������ �ǵ����� ����

    // Ÿ�̸� �ڵ�
    FTimerHandle RotationResetTimerHandle;

    virtual void BeginPlay() override;

    int HitCount = 0;

    UFUNCTION()
    void OnObjectHit();  // TriggerObject�� �¾��� �� ����

    // ȸ�� ���� ������� �ǵ����� �Լ�
    UFUNCTION()
    void ResetRotation();
};

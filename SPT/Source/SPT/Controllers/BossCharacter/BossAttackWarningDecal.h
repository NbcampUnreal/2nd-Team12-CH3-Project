// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"  // ��Į ������Ʈ
#include "Materials/MaterialInterface.h"  // ��Ƽ���� �������̽�
#include "BossAttackWarningDecal.generated.h"

UCLASS()
class SPT_API ABossAttackWarningDecal : public AActor
{
	GENERATED_BODY()
	
public:
    ABossAttackWarningDecal();
    virtual void Tick(float DeltaTime) override;
    // ù ��° ��Į ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* DecalComponent1;

    // �� ��° ��Į ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* DecalComponent2;

    // ��Į �ʱ�ȭ �Լ� (�� ���� ��Ƽ������ ���� �� �ֵ��� ����)
    void InitializeDecal(UMaterialInterface* InDecalMaterial1, UMaterialInterface* InDecalMaterial2, FVector Location, float W,float H, float Duration);

    // ��Į ���� �Լ�
    UFUNCTION()
    void DestroyDecal();

    // Ÿ�̸� �ڵ� (���� �ð� ó��)
    FTimerHandle DecalTimerHandle;

    float DecalRadius;
    float CurrentTime;
    float MaxDuration;

};

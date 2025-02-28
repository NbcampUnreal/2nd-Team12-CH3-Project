// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"  // 데칼 컴포넌트
#include "Materials/MaterialInterface.h"  // 머티리얼 인터페이스
#include "BossAttackWarningDecal.generated.h"

UCLASS()
class SPT_API ABossAttackWarningDecal : public AActor
{
	GENERATED_BODY()
	
public:
    ABossAttackWarningDecal();
    virtual void Tick(float DeltaTime) override;
    // 첫 번째 데칼 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* DecalComponent1;

    // 두 번째 데칼 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* DecalComponent2;

    // 데칼 초기화 함수 (두 개의 머티리얼을 받을 수 있도록 변경)
    void InitializeDecal(UMaterialInterface* InDecalMaterial1, UMaterialInterface* InDecalMaterial2, FVector Location, float W,float H, float Duration);

    // 데칼 삭제 함수
    UFUNCTION()
    void DestroyDecal();

    // 타이머 핸들 (지속 시간 처리)
    FTimerHandle DecalTimerHandle;

    float DecalRadius;
    float CurrentTime;
    float MaxDuration;

};

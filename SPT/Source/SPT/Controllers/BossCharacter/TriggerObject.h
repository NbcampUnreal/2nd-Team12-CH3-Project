// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitDelegate); // OnHit 이벤트 선언

UCLASS()
class SPT_API ATriggerObject : public AActor
{
	GENERATED_BODY()
	
public:
    // 기본 생성자
    ATriggerObject();

    // 메시 컴포넌트 (물리적인 형태를 나타냄)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // 충돌 감지를 위한 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    // OnHit 이벤트 (BossSpawnerTrigger에게 신호를 보냄)
    UPROPERTY(BlueprintAssignable)
    FOnHitDelegate OnHitTrigger;

    // 충돌 시 호출될 함수 (OnHit 이벤트 발생)
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
    
protected:
    // 게임 시작 시 호출되는 함수
    virtual void BeginPlay() override;

private:
    // 충돌 시 트리거를 발생시키는 함수
    void TriggerHit();
};
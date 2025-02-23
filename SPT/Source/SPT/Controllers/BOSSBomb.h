// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BOSSBomb.generated.h"

UCLASS()
class SPT_API ABOSSBomb : public AActor
{
    GENERATED_BODY()



public:
    virtual void BeginPlay() override;

    ABOSSBomb();

    // 충돌 감지를 위한 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    // 폭탄이 떨어지는 속도 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float BombSpeed = 1000.0f;


    // 폭탄이 충돌했을 때 실행될 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
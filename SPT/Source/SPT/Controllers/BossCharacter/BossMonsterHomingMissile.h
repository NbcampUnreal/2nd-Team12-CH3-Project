// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BossMonsterHomingMissile.generated.h"

UCLASS()
class SPT_API ABossMonsterHomingMissile : public AActor
{
	GENERATED_BODY()
	
public:
    ABossMonsterHomingMissile();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 플레이어를 추적하는 미사일
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    AActor* Target; // 플레이어 타겟

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;



    // 폭발 시 범위 내의 액터에게 피해 입히는 함수
    void Explode();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Missile")
    USphereComponent* ExplosionCollision;

    // 폭발 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float ExplosionDamage;


    // 충돌 시 처리
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    // 미사일의 속도 설정 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float MissileSpeed = 600.0f;

};

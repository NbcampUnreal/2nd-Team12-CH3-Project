// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossCharacterProjectile.generated.h"

UCLASS()
class SPT_API ABossCharacterProjectile : public AActor
{
	GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABossCharacterProjectile();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 충돌 감지를 위한 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    // 발사체 이동을 위한 컴포넌트
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovement;

    // 발사체가 충돌했을 때 실행될 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* ShotParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* ShotSound;

    FTimerHandle DestroyParticleTimerHandle; // 클래스 멤버 변수로 이동
};

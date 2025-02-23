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

    // �浹 ������ ���� ������Ʈ
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    // ��ź�� �������� �ӵ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb")
    float BombSpeed = 1000.0f;


    // ��ź�� �浹���� �� ����� �Լ�
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
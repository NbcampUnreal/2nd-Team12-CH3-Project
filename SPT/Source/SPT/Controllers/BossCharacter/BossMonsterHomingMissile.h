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

    // �÷��̾ �����ϴ� �̻���
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    AActor* Target; // �÷��̾� Ÿ��

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;



    // ���� �� ���� ���� ���Ϳ��� ���� ������ �Լ�
    void Explode();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Missile")
    USphereComponent* ExplosionCollision;

    // ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float ExplosionDamage;


    // �浹 �� ó��
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    // �̻����� �ӵ� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
    float MissileSpeed = 600.0f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossMonsterBomb.generated.h"

UCLASS()
class SPT_API ABossMonsterBomb : public AActor
{
	GENERATED_BODY()
	
public:	
    virtual void BeginPlay() override;

	// Sets default values for this actor's properties
	ABossMonsterBomb();

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




    /** ���� �ݰ� */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionRadius = 50.0f;

    /** ���� ���ط� */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionDamage = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* ExplosionCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* ExplosionSound;

    FTimerHandle DestroyParticleTimerHandle; // Ŭ���� ��� ������ �̵�

    void Explode();
};

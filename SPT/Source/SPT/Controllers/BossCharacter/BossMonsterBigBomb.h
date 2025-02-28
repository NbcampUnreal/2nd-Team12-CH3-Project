// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossMonsterBigBomb.generated.h"

class UProjectileMovementComponent;
class UDecalComponent;
class USphereComponent;


UCLASS()
class SPT_API ABossMonsterBigBomb : public AActor
{
	GENERATED_BODY()
	
public:
    ABossMonsterBigBomb();

protected:
    virtual void BeginPlay() override;

    /** �浹 �� ȣ��� �Լ� */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** ���� ���� */
    void Explode();

    /** �ٴڿ� ��� ��Į�� ǥ�� */
    void ShowExplosionWarning();

    /** ���� ������ ��Ÿ���� ��Į */
    UPROPERTY()
    UDecalComponent* WarningDecal;

    /** �浹 ������ ���� �ݸ��� */
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    /** �̵��� ����ϴ� ������Ʈ */
    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    /** �ٴ� ��� ǥ�ÿ� ��Ƽ���� */
    
    //Base
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UMaterialInterface* FirstMaterial;

    //�ð��� ���� Ȯ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UMaterialInterface* SecondMaterial;


    /** ���� �ݰ� */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionRadius = 500.0f;

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


};

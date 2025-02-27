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

    /** 충돌 시 호출될 함수 */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** 폭발 로직 */
    void Explode();

    /** 바닥에 경고 데칼을 표시 */
    void ShowExplosionWarning();

    /** 폭발 범위를 나타내는 데칼 */
    UPROPERTY()
    UDecalComponent* WarningDecal;

    /** 충돌 감지를 위한 콜리전 */
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    /** 이동을 담당하는 컴포넌트 */
    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    /** 바닥 경고 표시용 머티리얼 */
    
    //Base
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UMaterialInterface* FirstMaterial;

    //시간에 따라 확장
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UMaterialInterface* SecondMaterial;


    /** 폭발 반경 */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionRadius = 500.0f;

    /** 폭발 피해량 */
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float ExplosionDamage = 100.0f;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* ExplosionCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* ExplosionSound;

    FTimerHandle DestroyParticleTimerHandle; // 클래스 멤버 변수로 이동


};

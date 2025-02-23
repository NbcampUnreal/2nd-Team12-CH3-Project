#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HomingMissile.generated.h"


UCLASS()
class SPT_API AHomingMissile : public AActor
{
	GENERATED_BODY()

public:
    AHomingMissile();

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

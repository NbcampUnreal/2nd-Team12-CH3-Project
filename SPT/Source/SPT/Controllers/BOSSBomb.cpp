// Fill out your copyright notice in the Description page of Project Settings.

#include "BOSSBomb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABOSSBomb::ABOSSBomb()
{
    PrimaryActorTick.bCanEverTick = true;

    // �޽� ������Ʈ ����

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;    
    CollisionComponent->SetSphereRadius(10.0f);

    // �浹 �̺�Ʈ Ȱ��ȭ
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABOSSBomb::OnHit);


    // �߻�ü �̵� ������Ʈ ����
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = BombSpeed;
    ProjectileMovement->MaxSpeed = BombSpeed;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->bRotationFollowsVelocity = true;
}

void ABOSSBomb::BeginPlay()
{
    Super::BeginPlay();
}


void ABOSSBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this) // �ڱ� �ڽ� ����
    {
        // ������ ����
        UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);

        // �߻�ü �ı�
        Destroy();
    }
}
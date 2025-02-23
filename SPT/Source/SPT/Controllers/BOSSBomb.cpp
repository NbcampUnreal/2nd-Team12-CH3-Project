// Fill out your copyright notice in the Description page of Project Settings.

#include "BOSSBomb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABOSSBomb::ABOSSBomb()
{
    PrimaryActorTick.bCanEverTick = true;

    // 메시 컴포넌트 설정

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;    
    CollisionComponent->SetSphereRadius(10.0f);

    // 충돌 이벤트 활성화
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABOSSBomb::OnHit);


    // 발사체 이동 컴포넌트 설정
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
    if (OtherActor && OtherActor != this) // 자기 자신 제외
    {
        // 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);

        // 발사체 파괴
        Destroy();
    }
}
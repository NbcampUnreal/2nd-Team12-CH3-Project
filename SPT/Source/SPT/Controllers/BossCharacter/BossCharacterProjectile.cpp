// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossCharacterProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossCharacterProjectile::ABossCharacterProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(10.0f);

    // 충돌 이벤트 활성화
    CollisionComponent->SetNotifyRigidBodyCollision(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECC_PhysicsBody);

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossCharacterProjectile::OnHit);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 10000.0f;
    ProjectileMovement->MaxSpeed = 20000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABossCharacterProjectile::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ABossCharacterProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABossCharacterProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
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
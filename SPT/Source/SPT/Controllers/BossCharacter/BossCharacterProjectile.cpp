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

    UParticleSystemComponent* Particle = nullptr; // 파티클 컴포넌트 초기화

    if (ShotParticle)  // ExplosionParticle이 파티클 시스템 에셋일 때
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),              // 월드
            ShotParticle,       // 파티클 시스템 에셋
            GetActorLocation(),      // 파티클이 생성될 위치
            GetActorRotation(),      // 파티클이 생성될 회전
            false                    // 월드 공간에서 생성할지 여부 (true = 월드 공간, false = 로컬 공간)
        );
    }

    // 사운드 재생
    if (ShotSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ShotSound,
            GetActorLocation()
        );
    }
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
    if (OtherActor == GetOwner())
    {
        return;
    }
    if (OtherActor && OtherActor != this) // 자기 자신 제외
    {
        FString Message = "Hitted!";
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
        // 데미지 적용
        UGameplayStatics::ApplyDamage(OtherActor, 50.0f, nullptr, this, nullptr);
    }
    // 발사체 파괴
    Destroy();
}
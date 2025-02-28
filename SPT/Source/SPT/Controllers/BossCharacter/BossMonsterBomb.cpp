// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossMonsterBomb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABossMonsterBomb::ABossMonsterBomb()
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

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterBomb::OnHit);


    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);


    // 발사체 이동 컴포넌트 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 0;
    ProjectileMovement->MaxSpeed = 0;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->bRotationFollowsVelocity = true;
}

void ABossMonsterBomb::BeginPlay()
{
    Super::BeginPlay();
}


void ABossMonsterBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    Explode();
}


void ABossMonsterBomb::Explode()
{
    UParticleSystemComponent* Particle = nullptr; // 파티클 컴포넌트 초기화

    if (ExplosionParticle)  // ExplosionParticle이 파티클 시스템 에셋일 때
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),              // 월드
            ExplosionParticle,       // 파티클 시스템 에셋
            GetActorLocation(),      // 파티클이 생성될 위치
            GetActorRotation(),      // 파티클이 생성될 회전
            false                    // 월드 공간에서 생성할지 여부 (true = 월드 공간, false = 로컬 공간)
        );
    }

    // 충돌 감지 설정 보장
    ExplosionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    ExplosionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );
        }
    }

    // 사운드 재생
    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            ExplosionSound,
            GetActorLocation()
        );
    }

    Destroy();
}
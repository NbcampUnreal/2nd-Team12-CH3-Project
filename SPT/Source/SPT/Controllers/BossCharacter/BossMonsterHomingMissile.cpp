// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossMonsterHomingMissile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h" 
#include "CollisionQueryParams.h" 


ABossMonsterHomingMissile::ABossMonsterHomingMissile()
{
    PrimaryActorTick.bCanEverTick = true;

    ExplosionRadius = 1000.0f;
    ExplosionDamage = 30.0f;

    // 충돌 감지용 컴포넌트
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(15.0f);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterHomingMissile::OnHit);

    CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
    CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

    // 미사일 메시
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // 유도탄 이동 컴포넌트
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = MissileSpeed;
    ProjectileMovement->MaxSpeed = MissileSpeed;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->bRotationFollowsVelocity = true;

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);

    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void ABossMonsterHomingMissile::BeginPlay()
{
    Super::BeginPlay();

    // 플레이어를 타겟으로 설정
    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Target)
    {
        // 플레이어의 위치를 계산하여 미사일을 그쪽으로 회전시킴
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();

        // 미사일이 수평으로 회전하도록 설정
        SetActorRotation(FRotator(0, NewRotation.Yaw, 0)); // Z축 회전만 적용하여 수평 회전


        ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingAccelerationMagnitude = 3000.0f;
    }
}

void ABossMonsterHomingMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 유도탄이 목표를 추적하도록 계속 업데이트
    if (Target)
    {
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();
        SetActorRotation(NewRotation);
    }
}



void ABossMonsterHomingMissile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    Explode();
}

void ABossMonsterHomingMissile::Explode()
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossMonsterBigBomb.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BossCharacter/BossAttackWarningDecal.h"
#include "Components/DecalComponent.h"

ABossMonsterBigBomb::ABossMonsterBigBomb()
{
    PrimaryActorTick.bCanEverTick = false;

    // 콜리전 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionComponent;

    // 충돌 이벤트 바인딩
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterBigBomb::OnHit);

    // 이동 컴포넌트 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 0.0f;
    ProjectileMovement->MaxSpeed = 0.0f;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 1.0f; // 중력 적용

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);

}

void ABossMonsterBigBomb::BeginPlay()
{
    Super::BeginPlay();
    FString Message1 = "Decal Created";

    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    // 바닥에 경고 표시
    //ShowExplosionWarning();
}

void ABossMonsterBigBomb::ShowExplosionWarning()
{
    if (!FirstMaterial || !SecondMaterial) return;

    FString Message = "DecalMaterial Existed";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);

    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation - FVector(0, 0, 5000.0f); // 아래로 Raycast

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
    {
        FVector GroundLocation = HitResult.ImpactPoint + FVector(0, 0, 1.0f); // 바닥보다 살짝 위로

        // 커스텀 데칼 액터 생성
        FString Message1 = "Decal Actor Created";
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
        ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
        if (WarningDecalActor)
        {
            // 빨간색 머티리얼을 데칼에 적용
            WarningDecalActor->InitializeDecal(FirstMaterial, SecondMaterial, GroundLocation, ExplosionRadius, ExplosionRadius, 3.0f);
        }
    }
    
}

void ABossMonsterBigBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 충돌 시 폭발
    Explode();
}

void ABossMonsterBigBomb::Explode()
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
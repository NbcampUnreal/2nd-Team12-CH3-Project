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

    // �浹 ������ ������Ʈ
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(15.0f);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterHomingMissile::OnHit);

    CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
    CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

    // �̻��� �޽�
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    // ����ź �̵� ������Ʈ
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

    // �÷��̾ Ÿ������ ����
    Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (Target)
    {
        // �÷��̾��� ��ġ�� ����Ͽ� �̻����� �������� ȸ����Ŵ
        FVector Direction = Target->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();

        // �̻����� �������� ȸ���ϵ��� ����
        SetActorRotation(FRotator(0, NewRotation.Yaw, 0)); // Z�� ȸ���� �����Ͽ� ���� ȸ��


        ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingAccelerationMagnitude = 3000.0f;
    }
}

void ABossMonsterHomingMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ����ź�� ��ǥ�� �����ϵ��� ��� ������Ʈ
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
    UParticleSystemComponent* Particle = nullptr; // ��ƼŬ ������Ʈ �ʱ�ȭ

    if (ExplosionParticle)  // ExplosionParticle�� ��ƼŬ �ý��� ������ ��
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),              // ����
            ExplosionParticle,       // ��ƼŬ �ý��� ����
            GetActorLocation(),      // ��ƼŬ�� ������ ��ġ
            GetActorRotation(),      // ��ƼŬ�� ������ ȸ��
            false                    // ���� �������� �������� ���� (true = ���� ����, false = ���� ����)
        );
    }

    // �浹 ���� ���� ����
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

    // ���� ���
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

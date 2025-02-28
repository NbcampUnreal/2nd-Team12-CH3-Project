// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter/BossMonsterBomb.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABossMonsterBomb::ABossMonsterBomb()
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

    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterBomb::OnHit);


    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);


    // �߻�ü �̵� ������Ʈ ����
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
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

    // �ݸ��� ����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    RootComponent = CollisionComponent;

    // �浹 �̺�Ʈ ���ε�
    CollisionComponent->OnComponentHit.AddDynamic(this, &ABossMonsterBigBomb::OnHit);

    // �̵� ������Ʈ ����
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 0.0f;
    ProjectileMovement->MaxSpeed = 0.0f;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 1.0f; // �߷� ����

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
    // �ٴڿ� ��� ǥ��
    //ShowExplosionWarning();
}

void ABossMonsterBigBomb::ShowExplosionWarning()
{
    if (!FirstMaterial || !SecondMaterial) return;

    FString Message = "DecalMaterial Existed";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);

    FVector StartLocation = GetActorLocation();
    FVector EndLocation = StartLocation - FVector(0, 0, 5000.0f); // �Ʒ��� Raycast

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
    {
        FVector GroundLocation = HitResult.ImpactPoint + FVector(0, 0, 1.0f); // �ٴں��� ��¦ ����

        // Ŀ���� ��Į ���� ����
        FString Message1 = "Decal Actor Created";
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
        ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
        if (WarningDecalActor)
        {
            // ������ ��Ƽ������ ��Į�� ����
            WarningDecalActor->InitializeDecal(FirstMaterial, SecondMaterial, GroundLocation, ExplosionRadius, ExplosionRadius, 3.0f);
        }
    }
    
}

void ABossMonsterBigBomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // �浹 �� ����
    Explode();
}

void ABossMonsterBigBomb::Explode()
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
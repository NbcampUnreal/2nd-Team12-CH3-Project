#include "SPTBossCharacter.h"
#include "SPTBossCharacterAIController.h"
#include "Bosscharacter/BossMonsterHomingMissile.h"
#include "Kismet/GameplayStatics.h"
#include "BossCharacter/BossCharacterProjectile.h"
#include "Bosscharacter/BossMonsterBomb.h"
#include "Bosscharacter/BossMonsterBigBomb.h"
#include "BossCharacter/BossAttackWarningDecal.h"

// Sets default values
ASPTBossCharacter::ASPTBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// �� ĳ���͸� ��Ʈ���� AIController Ŭ������ EnemyAIController�� ����
	AIControllerClass = ASPTBossCharacterAIController::StaticClass();
	// ������ ��ġ�ǰų� �����Ǹ� �ڵ����� AIController�� �Ҵ�ǵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
    HP = 100;
    CurrentBulletCount = 0;
	// �߻� ��ġ�� ������ �⺻ ������ ��
	MissileSpawnOffset = FVector(0.0f, 0.0f, 150.0f); // ����: ������ �Ӹ� ������ �߻�

    CurrentMissileCount = 0;
}

// Called when the game starts or when spawned
void ASPTBossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPTBossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASPTBossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int ASPTBossCharacter::GetHP()
{
	return HP;
}

void ASPTBossCharacter::SetHP(int Amount)
{
	HP = Amount;
}

void ASPTBossCharacter::StartMissileAttack()
{
    CurrentMissileCount = 0; // ���� �߻�� �̻��� �� �ʱ�ȭ

    // Ÿ�̸Ӹ� �����Ͽ� ���� �������� �̻����� �߻�
    GetWorldTimerManager().SetTimer(MissileTimerHandle, this, &ASPTBossCharacter::SpawnMissile, 1.0f, true, 0.0f);
}

void ASPTBossCharacter::SpawnMissile()
{
    if (MissileClass && CurrentMissileCount < MissileCount)
    {
        AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerActor)
        {
            // �⺻ ���� ��ġ
            FVector BaseSpawnLocation = GetActorLocation() + MissileSpawnOffset;
            FVector RandomOffset = FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(500.0f, 1000.0f));
            FVector SpawnLocation = BaseSpawnLocation + RandomOffset;

            // �÷��̾� ������ �ٶ󺸴� ȸ���� ���
            FVector Direction = PlayerActor->GetActorLocation() - SpawnLocation;
            FRotator SpawnRotation = Direction.Rotation();

            // �̻��� ����
            ABossMonsterHomingMissile* NewMissile = GetWorld()->SpawnActor<ABossMonsterHomingMissile>(MissileClass, SpawnLocation, SpawnRotation);
            if (NewMissile)
            {
                NewMissile->MissileSpeed = MissileSpeed;
            }

            // ���� �߻�� �̻��� �� ����
            CurrentMissileCount++;

            // ��� �̻����� �߻������� Ÿ�̸� ����
            if (CurrentMissileCount >= MissileCount)
            {
                GetWorldTimerManager().ClearTimer(MissileTimerHandle);
            }
        }
    }
}

void ASPTBossCharacter::SnipingPlayer()
{
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;

    // 1. �÷��̾� �������� ȸ��
    FRotator LookAtRotation = (PlayerActor->GetActorLocation() - GetActorLocation()).Rotation();
    SetActorRotation(LookAtRotation);

    // 2. ���� ���� ����
    bIsAiming = true;

    // 3. ���� �ð� �� ���� ����
    GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ASPTBossCharacter::FireProjectile, AimDuration, false);

    // 4. ���ؼ� �׸���
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::StartAiming()
{
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;

    // ���ؼ� �׸���
    GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ASPTBossCharacter::FireProjectile, AimDuration, false);

    // Ÿ�� �������� ȸ�� (�÷��̾ ����)
    FRotator LookAtRotation = (PlayerActor->GetActorLocation() - GetActorLocation()).Rotation();
    SetActorRotation(LookAtRotation);

    // ���� �ð� ���� ���ؼ� ����
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::DrawAimLine()
{
    if (!bIsAiming) return; // ���� ���� �ƴϸ� ���� �� ��

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;

    FVector Start = GetActorLocation() + FVector(0, 0, 20); // ���� �ѱ� ��ġ �������� ����
    FVector End = PlayerActor->GetActorLocation();

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.01f, 0, 2.0f);

    // ���� �����ӿ� �ٽ� ȣ���Ͽ� ���ؼ� ����
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::FireProjectile()
{
    bIsAiming = false; // �߻� �� ���ؼ� ����

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!ProjectileClass || !PlayerActor) return;

    // ������ �÷��̾ ���ϴ� ���� ���� ���
    FVector ForwardVector = (PlayerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    // ������ ���鿡�� �ణ ������ ��ġ���� �߻�
    FVector SpawnLocation = GetActorLocation() + ForwardVector * 100.0f; // ���� ���� 100���� ��
    SpawnLocation.Z += 50.0f; // �߻� ��ġ�� �Ӹ� ��ó�� ����

    // �÷��̾ ��Ȯ�� �����ϴ� ȸ�� �� ���
    FVector TargetLocation = PlayerActor->GetActorLocation();
    TargetLocation.Z += 40.0f; // ��ǥ ��ġ�� ��¦ �÷��� ��Ȯ�� ����

    FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

    // �߻�ü ����
    ABossCharacterProjectile* Projectile = GetWorld()->SpawnActor<ABossCharacterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        Projectile->SetOwner(this);

        // �������� �浹 ����
        UPrimitiveComponent* ProjectileCollision = Cast<UPrimitiveComponent>(Projectile->GetRootComponent());
        if (ProjectileCollision)
        {
            ProjectileCollision->IgnoreActorWhenMoving(this, true);
        }
    }
}



void ASPTBossCharacter::StartSpawnBombs()
{
    ShowSmallBombWarning();
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;
    FVector TargetLocation = PlayerActor->GetActorLocation();

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, FName("SpawnBombs"), TargetLocation);

    GetWorldTimerManager().SetTimer(SpawnSmallBombDelayTimerHandle, TimerDelegate, 2.0f, false);
}

void ASPTBossCharacter::SpawnBombs(FVector TargetLocation)
{
    // ������ ��ġ�� �÷��̾� ���� ��������
    FVector BossLocation = GetActorLocation();
    FVector BossForwardVector = (TargetLocation - GetActorLocation());

    // �÷��̾ ���� ���� ���
    FVector TargetDirection = BossForwardVector.GetSafeNormal();

    // ���� ���� ��ġ�� ���� ����
    int32 GridSizeX = 4; // X�� ���� ��ź ����
    int32 GridSizeY = 5; // Y�� ���� ��ź ����
    float GridSpacing = 400.0f; // ���� ����

    // ������ �Ǵ� ���� ��� �𼭸� ���
    FVector GridOrigin = TargetLocation - TargetDirection * 1500.0f
        - FVector((GridSizeX - 1) * 0.5f * GridSpacing, (GridSizeY - 1) * 0.5f * GridSpacing, 0);

    for (int32 x = 0; x < GridSizeX; ++x)
    {
        for (int32 y = 0; y < GridSizeY; ++y)
        {
            // ���� ��ġ ���
            FVector SpawnPosition = GridOrigin + FVector(x * GridSpacing, y * GridSpacing, 2000.0f);

            // ��ź�� �ʱ� ȸ�� �� (�÷��̾� ����)
            FRotator SpawnRotation = TargetDirection.Rotation() - FRotator(0.0f, 0.0f, 80.0f);

            // ��ź ����
            ABossMonsterBomb* NewBomb = GetWorld()->SpawnActor<ABossMonsterBomb>(BombClass, SpawnPosition, SpawnRotation);
            if (NewBomb)
            {
                // ��ź�� �÷��̾ ���� �̵�
                NewBomb->BombSpeed = 2000.0f;
                FVector BombVelocity = TargetDirection * NewBomb->BombSpeed + FVector(0, 0, -2000);
                NewBomb->ProjectileMovement->Velocity = BombVelocity;
            }
        }
    }
}



void ASPTBossCharacter::SpawnBigBomb()
{
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FVector SpawnLocation = PlayerActor->GetActorLocation() + FVector(0, 0, 3000);  // ���߿��� ����
    FRotator SpawnRotation = FRotator::ZeroRotator;

    GetWorld()->SpawnActor<ABossMonsterBigBomb>(BigBombClass, SpawnLocation, SpawnRotation);
    ShowBigBombWarning();
}



void ASPTBossCharacter::StartRandomShooting()
{
    ShowFireRandomShotsWarning(); // ��� ǥ��

    CurrentBulletCount = 0; // �ʱ�ȭ

    // 1�� �� FireRandomShots ����
    GetWorldTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ASPTBossCharacter::StartFiring, 1.0f, false);
}

void ASPTBossCharacter::StartFiring()
{
    // Ÿ�̸Ӹ� �����Ͽ� ���� ���� ���������� �߻�
    GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ASPTBossCharacter::FireRandomShots, FireRate, true);
}

// �Ѿ� ���� �� �߻�
void ASPTBossCharacter::FireRandomShots()
{
    if (CurrentBulletCount >= BulletCount)
    {
        GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
        return;
    }

    // ������ ���� ��� (��ä�� ���� ������ ����)
    float RandomAngle = FMath::RandRange(-BulletSpreadAngle * 0.5f, BulletSpreadAngle * 0.5f);

    // ������ ���� ���� ��������
    FVector ForwardVector = GetActorForwardVector();
    FRotator BossRotation = GetActorRotation();

    // ���� ȸ�� ����
    FRotator BulletRotation = BossRotation + FRotator(0.0f, RandomAngle, 0.0f);

    // �߻� ��ġ ���� (������ ���鿡�� �ణ ������ ��)
    FVector SpawnLocation = GetActorLocation() + ForwardVector * 100.0f;

    // �Ѿ� ����
    SpawnBullet(SpawnLocation, BulletRotation);

    // ���� �߻�� �Ѿ� ���� ����
    CurrentBulletCount++;
}

// �Ѿ� ���� �Լ�
void ASPTBossCharacter::SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation)
{
    if (BulletClass)
    {
        FActorSpawnParameters SpawnParams;
        GetWorld()->SpawnActor<ABossCharacterProjectile>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
}

void ASPTBossCharacter::ShowFireRandomShotsWarning()
{
    if (!RandomShootingFirstMaterial || !RandomShootingSecondMaterial) return;

    FString Message = "DecalMaterial Existed";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);

    FVector StartLocation = GetActorLocation();
    StartLocation.Z = 0;
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // �ٴں��� ��¦ ����

    // Ŀ���� ��Į ���� ����
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // ������ ��Ƽ������ ��Į�� ����
        WarningDecalActor->InitializeDecal(RandomShootingFirstMaterial, RandomShootingSecondMaterial, GroundLocation, 5000.0f, 5000.0f, 1.0f);
        WarningDecalActor->SetActorRotation(GetActorRotation() - FRotator(0.0f, 22.5f, 0.0f));
    }

}

void ASPTBossCharacter::ShowBigBombWarning()
{
    if (!BigBombFirstMaterial || !BigBombSecondMaterial) return;

    FString Message = "DecalMaterial Existed";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FVector StartLocation = PlayerActor->GetActorLocation();
    StartLocation.Z = 0;
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // �ٴں��� ��¦ ����

    // Ŀ���� ��Į ���� ����
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // ������ ��Ƽ������ ��Į�� ����
        WarningDecalActor->InitializeDecal(BigBombFirstMaterial, BigBombSecondMaterial, GroundLocation, 2500.0f, 2500.0f, 2.3f);
    }
}



void ASPTBossCharacter::ShowSmallBombWarning()
{
    if (!SmallBombFirstMaterial || !SmallBombSecondMaterial) return;

    FString Message = "DecalMaterial Existed";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    FVector StartLocation = PlayerActor->GetActorLocation();
    StartLocation.Z = 0;
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // �ٴں��� ��¦ ����

    // Ŀ���� ��Į ���� ����
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // ������ ��Ƽ������ ��Į�� ����
        WarningDecalActor->InitializeDecal(SmallBombFirstMaterial, SmallBombSecondMaterial, GroundLocation, 1500.0f, 1000.0f,2.3f);
    }
}
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
	// 이 캐릭터를 컨트롤할 AIController 클래스를 EnemyAIController로 지정
	AIControllerClass = ASPTBossCharacterAIController::StaticClass();
	// 레벨에 배치되거나 스폰되면 자동으로 AIController가 할당되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
    HP = 100;
    CurrentBulletCount = 0;
	// 발사 위치를 설정할 기본 오프셋 값
	MissileSpawnOffset = FVector(0.0f, 0.0f, 150.0f); // 예시: 보스의 머리 위에서 발사

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
    CurrentMissileCount = 0; // 현재 발사된 미사일 수 초기화

    // 타이머를 설정하여 일정 간격으로 미사일을 발사
    GetWorldTimerManager().SetTimer(MissileTimerHandle, this, &ASPTBossCharacter::SpawnMissile, 1.0f, true, 0.0f);
}

void ASPTBossCharacter::SpawnMissile()
{
    if (MissileClass && CurrentMissileCount < MissileCount)
    {
        AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerActor)
        {
            // 기본 스폰 위치
            FVector BaseSpawnLocation = GetActorLocation() + MissileSpawnOffset;
            FVector RandomOffset = FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(500.0f, 1000.0f));
            FVector SpawnLocation = BaseSpawnLocation + RandomOffset;

            // 플레이어 방향을 바라보는 회전값 계산
            FVector Direction = PlayerActor->GetActorLocation() - SpawnLocation;
            FRotator SpawnRotation = Direction.Rotation();

            // 미사일 생성
            ABossMonsterHomingMissile* NewMissile = GetWorld()->SpawnActor<ABossMonsterHomingMissile>(MissileClass, SpawnLocation, SpawnRotation);
            if (NewMissile)
            {
                NewMissile->MissileSpeed = MissileSpeed;
            }

            // 현재 발사된 미사일 수 증가
            CurrentMissileCount++;

            // 모든 미사일을 발사했으면 타이머 중지
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

    // 1. 플레이어 방향으로 회전
    FRotator LookAtRotation = (PlayerActor->GetActorLocation() - GetActorLocation()).Rotation();
    SetActorRotation(LookAtRotation);

    // 2. 조준 상태 설정
    bIsAiming = true;

    // 3. 일정 시간 후 공격 실행
    GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ASPTBossCharacter::FireProjectile, AimDuration, false);

    // 4. 조준선 그리기
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::StartAiming()
{
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;

    // 조준선 그리기
    GetWorld()->GetTimerManager().SetTimer(AimTimerHandle, this, &ASPTBossCharacter::FireProjectile, AimDuration, false);

    // 타겟 방향으로 회전 (플레이어를 조준)
    FRotator LookAtRotation = (PlayerActor->GetActorLocation() - GetActorLocation()).Rotation();
    SetActorRotation(LookAtRotation);

    // 일정 시간 동안 조준선 갱신
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::DrawAimLine()
{
    if (!bIsAiming) return; // 조준 중이 아니면 실행 안 함

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerActor) return;

    FVector Start = GetActorLocation() + FVector(0, 0, 20); // 보스 총구 위치 즈음에서 시작
    FVector End = PlayerActor->GetActorLocation();

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.01f, 0, 2.0f);

    // 다음 프레임에 다시 호출하여 조준선 유지
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASPTBossCharacter::DrawAimLine);
}

void ASPTBossCharacter::FireProjectile()
{
    bIsAiming = false; // 발사 후 조준선 정지

    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!ProjectileClass || !PlayerActor) return;

    // 보스가 플레이어를 향하는 방향 벡터 계산
    FVector ForwardVector = (PlayerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();

    // 보스의 정면에서 약간 떨어진 위치에서 발사
    FVector SpawnLocation = GetActorLocation() + ForwardVector * 100.0f; // 보스 정면 100유닛 앞
    SpawnLocation.Z += 50.0f; // 발사 위치를 머리 근처로 조정

    // 플레이어를 정확히 조준하는 회전 값 계산
    FVector TargetLocation = PlayerActor->GetActorLocation();
    TargetLocation.Z += 40.0f; // 목표 위치를 살짝 올려서 정확한 조준

    FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

    // 발사체 스폰
    ABossCharacterProjectile* Projectile = GetWorld()->SpawnActor<ABossCharacterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        Projectile->SetOwner(this);

        // 보스와의 충돌 방지
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
    // 보스의 위치와 플레이어 방향 가져오기
    FVector BossLocation = GetActorLocation();
    FVector BossForwardVector = (TargetLocation - GetActorLocation());

    // 플레이어를 향한 방향 계산
    FVector TargetDirection = BossForwardVector.GetSafeNormal();

    // 격자 형태 배치를 위한 설정
    int32 GridSizeX = 4; // X축 방향 폭탄 개수
    int32 GridSizeY = 5; // Y축 방향 폭탄 개수
    float GridSpacing = 400.0f; // 격자 간격

    // 기준이 되는 왼쪽 상단 모서리 계산
    FVector GridOrigin = TargetLocation - TargetDirection * 1500.0f
        - FVector((GridSizeX - 1) * 0.5f * GridSpacing, (GridSizeY - 1) * 0.5f * GridSpacing, 0);

    for (int32 x = 0; x < GridSizeX; ++x)
    {
        for (int32 y = 0; y < GridSizeY; ++y)
        {
            // 격자 위치 계산
            FVector SpawnPosition = GridOrigin + FVector(x * GridSpacing, y * GridSpacing, 2000.0f);

            // 폭탄의 초기 회전 값 (플레이어 방향)
            FRotator SpawnRotation = TargetDirection.Rotation() - FRotator(0.0f, 0.0f, 80.0f);

            // 폭탄 생성
            ABossMonsterBomb* NewBomb = GetWorld()->SpawnActor<ABossMonsterBomb>(BombClass, SpawnPosition, SpawnRotation);
            if (NewBomb)
            {
                // 폭탄이 플레이어를 향해 이동
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
    FVector SpawnLocation = PlayerActor->GetActorLocation() + FVector(0, 0, 3000);  // 공중에서 생성
    FRotator SpawnRotation = FRotator::ZeroRotator;

    GetWorld()->SpawnActor<ABossMonsterBigBomb>(BigBombClass, SpawnLocation, SpawnRotation);
    ShowBigBombWarning();
}



void ASPTBossCharacter::StartRandomShooting()
{
    ShowFireRandomShotsWarning(); // 경고 표시

    CurrentBulletCount = 0; // 초기화

    // 1초 후 FireRandomShots 실행
    GetWorldTimerManager().SetTimer(ShootingDelayTimerHandle, this, &ASPTBossCharacter::StartFiring, 1.0f, false);
}

void ASPTBossCharacter::StartFiring()
{
    // 타이머를 설정하여 여러 발을 순차적으로 발사
    GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ASPTBossCharacter::FireRandomShots, FireRate, true);
}

// 총알 여러 개 발사
void ASPTBossCharacter::FireRandomShots()
{
    if (CurrentBulletCount >= BulletCount)
    {
        GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
        return;
    }

    // 무작위 각도 계산 (부채꼴 범위 내에서 랜덤)
    float RandomAngle = FMath::RandRange(-BulletSpreadAngle * 0.5f, BulletSpreadAngle * 0.5f);

    // 보스의 정면 방향 가져오기
    FVector ForwardVector = GetActorForwardVector();
    FRotator BossRotation = GetActorRotation();

    // 랜덤 회전 적용
    FRotator BulletRotation = BossRotation + FRotator(0.0f, RandomAngle, 0.0f);

    // 발사 위치 설정 (보스의 정면에서 약간 떨어진 곳)
    FVector SpawnLocation = GetActorLocation() + ForwardVector * 100.0f;

    // 총알 스폰
    SpawnBullet(SpawnLocation, BulletRotation);

    // 현재 발사된 총알 개수 증가
    CurrentBulletCount++;
}

// 총알 생성 함수
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
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // 바닥보다 살짝 위로

    // 커스텀 데칼 액터 생성
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // 빨간색 머티리얼을 데칼에 적용
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
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // 바닥보다 살짝 위로

    // 커스텀 데칼 액터 생성
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // 빨간색 머티리얼을 데칼에 적용
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
    FVector GroundLocation = StartLocation + FVector(0, 0, 1.0f); // 바닥보다 살짝 위로

    // 커스텀 데칼 액터 생성
    FString Message1 = "Decal Actor Created";
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message1);
    ABossAttackWarningDecal* WarningDecalActor = GetWorld()->SpawnActor<ABossAttackWarningDecal>(ABossAttackWarningDecal::StaticClass(), GroundLocation, FRotator::ZeroRotator);
    if (WarningDecalActor)
    {
        // 빨간색 머티리얼을 데칼에 적용
        WarningDecalActor->InitializeDecal(SmallBombFirstMaterial, SmallBombSecondMaterial, GroundLocation, 1500.0f, 1000.0f,2.3f);
    }
}
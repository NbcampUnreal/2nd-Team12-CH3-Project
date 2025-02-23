#include "SPTBossCharacter.h"
#include "SPTBossCharacterAIController.h"
#include "HomingMissile.h"
#include "Kismet/GameplayStatics.h"
#include "SPTBOSSProjectile.h"
#include "BOSSBomb.h"

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

	// 발사 위치를 설정할 기본 오프셋 값
	MissileSpawnOffset = FVector(0.0f, 0.0f, 150.0f); // 예시: 보스의 머리 위에서 발사
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

void ASPTBossCharacter::SpawnMissile()
{
    if (MissileClass)
    {
        // 플레이어를 타겟으로 설정 (플레이어는 AActor 클래스)
        AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerActor)
        {
            // 미사일 갯수만큼 반복
            for (int i = 0; i < MissileCount; i++)
            {
                // 기본 스폰 위치 계산 (보스의 위치 + 오프셋)
                FVector BaseSpawnLocation = GetActorLocation() + MissileSpawnOffset;

                // 오프셋에 랜덤 값을 추가 (랜덤 범위: -50 ~ 50, 조정 가능)
                FVector RandomOffset = FVector(FMath::RandRange(-50.0f, 50.0f), FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(0.0f, 50.0f));
                FVector SpawnLocation = BaseSpawnLocation + RandomOffset; // 랜덤 위치를 더해줌

                // 플레이어 방향을 향하는 회전값 계산
                FVector Direction = PlayerActor->GetActorLocation() - SpawnLocation; // 플레이어 방향 계산
                FRotator SpawnRotation = Direction.Rotation(); // 플레이어를 바라보는 회전값 계산

                // 미사일 스폰
                AHomingMissile* NewMissile = GetWorld()->SpawnActor<AHomingMissile>(MissileClass, SpawnLocation, SpawnRotation);
                if (NewMissile)
                {
                    // 미사일의 속도나 기타 설정
                    NewMissile->MissileSpeed = MissileSpeed;
                }
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

    FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 50);
    FRotator SpawnRotation = (PlayerActor->GetActorLocation() - SpawnLocation).Rotation();

    ASPTBOSSProjectile* Projectile = GetWorld()->SpawnActor<ASPTBOSSProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    if (Projectile)
    {
        Projectile->SetOwner(this);
    }
}


void ASPTBossCharacter::SpawnBombs()
{
    AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    BombSpawnLocation = PlayerActor->GetActorLocation();
    for (int32 i = 0; i < BombCount; ++i)
    {
        // 랜덤한 위치 계산
        FVector SpawnPosition = BombSpawnLocation + FVector(FMath::RandRange(-500.f, 500.f), FMath::RandRange(-500.f, 500.f), 1000.0f);

        FRotator SpawnRotation = FRotator::ZeroRotator;  // 미사일의 초기 회전 값
            
        // 미사일 생성
        ABOSSBomb* NewBomb = GetWorld()->SpawnActor<ABOSSBomb>(BombClass, SpawnPosition, SpawnRotation);
        if (NewBomb)
        {
            // 미사일의 속도나 다른 설정을 블루프린트에서 조정할 수 있도록 설정
            NewBomb->BombSpeed = 1000.0f;

            // 일정 시간 후 폭탄이 떨어지도록 설정
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewBomb]() {
                NewBomb->ProjectileMovement->SetVelocityInLocalSpace(FVector(0, 0, -1) * NewBomb->BombSpeed);  // 아래로 떨어지도록 설정
                }, BombDropInterval, false);
        }
    }
}
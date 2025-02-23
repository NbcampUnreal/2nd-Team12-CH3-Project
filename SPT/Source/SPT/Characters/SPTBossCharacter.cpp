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
	// �� ĳ���͸� ��Ʈ���� AIController Ŭ������ EnemyAIController�� ����
	AIControllerClass = ASPTBossCharacterAIController::StaticClass();
	// ������ ��ġ�ǰų� �����Ǹ� �ڵ����� AIController�� �Ҵ�ǵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HP = 100;

	// �߻� ��ġ�� ������ �⺻ ������ ��
	MissileSpawnOffset = FVector(0.0f, 0.0f, 150.0f); // ����: ������ �Ӹ� ������ �߻�
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
        // �÷��̾ Ÿ������ ���� (�÷��̾�� AActor Ŭ����)
        AActor* PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerActor)
        {
            // �̻��� ������ŭ �ݺ�
            for (int i = 0; i < MissileCount; i++)
            {
                // �⺻ ���� ��ġ ��� (������ ��ġ + ������)
                FVector BaseSpawnLocation = GetActorLocation() + MissileSpawnOffset;

                // �����¿� ���� ���� �߰� (���� ����: -50 ~ 50, ���� ����)
                FVector RandomOffset = FVector(FMath::RandRange(-50.0f, 50.0f), FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(0.0f, 50.0f));
                FVector SpawnLocation = BaseSpawnLocation + RandomOffset; // ���� ��ġ�� ������

                // �÷��̾� ������ ���ϴ� ȸ���� ���
                FVector Direction = PlayerActor->GetActorLocation() - SpawnLocation; // �÷��̾� ���� ���
                FRotator SpawnRotation = Direction.Rotation(); // �÷��̾ �ٶ󺸴� ȸ���� ���

                // �̻��� ����
                AHomingMissile* NewMissile = GetWorld()->SpawnActor<AHomingMissile>(MissileClass, SpawnLocation, SpawnRotation);
                if (NewMissile)
                {
                    // �̻����� �ӵ��� ��Ÿ ����
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
        // ������ ��ġ ���
        FVector SpawnPosition = BombSpawnLocation + FVector(FMath::RandRange(-500.f, 500.f), FMath::RandRange(-500.f, 500.f), 1000.0f);

        FRotator SpawnRotation = FRotator::ZeroRotator;  // �̻����� �ʱ� ȸ�� ��
            
        // �̻��� ����
        ABOSSBomb* NewBomb = GetWorld()->SpawnActor<ABOSSBomb>(BombClass, SpawnPosition, SpawnRotation);
        if (NewBomb)
        {
            // �̻����� �ӵ��� �ٸ� ������ �������Ʈ���� ������ �� �ֵ��� ����
            NewBomb->BombSpeed = 1000.0f;

            // ���� �ð� �� ��ź�� ���������� ����
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [NewBomb]() {
                NewBomb->ProjectileMovement->SetVelocityInLocalSpace(FVector(0, 0, -1) * NewBomb->BombSpeed);  // �Ʒ��� ���������� ����
                }, BombDropInterval, false);
        }
    }
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "SPTBossCharacter.generated.h"

UCLASS()
class SPT_API ASPTBossCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPTBossCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

#pragma region CharacterDefaultSettings_Test

	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetHP();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHP(float Amount);

#pragma endregion


#pragma region BossCharacterSpecialAttackSettings

	/** 폭발 범위를 나타내는 데칼 */
	UPROPERTY()
	UDecalComponent* WarningDecal;

#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnMissiles
	//////변수//////
	int CurrentMissileCount;

	FTimerHandle MissileTimerHandle; // 미사일 발사 간격 컨트롤 타이머

	// 미사일 발사 위치를 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	FVector MissileSpawnOffset; // 미사일의 발사 위치 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	int32 MissileCount = 3; // 미사일 갯수 (블루프린트에서 설정 가능)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	TSubclassOf<AActor> MissileClass; // 미사일 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	float MissileSpeed = 600.0f; // 미사일 속도

	//////함수//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnMissile")
	void SpawnMissile();

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnMissile")
	void StartMissileAttack();

#pragma endregion


#pragma region BossCharacterSpecialAttacks_Sniping
	//////변수//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	float AimDuration = 2.0f; // 조준 지속 시간

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	TSubclassOf<AActor> ProjectileClass; // 발사할 총알 클래스

	bool bIsAiming = false;  // 현재 조준 중인지 확인하는 변수

	FTimerHandle AimTimerHandle; //조준 후 발사까지의 경과시간 컨트롤 타이머

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UStaticMeshComponent* AimLineMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UStaticMesh* CylinderMesh;

	// 빨간색 머티리얼
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UMaterialInterface* RedMaterial;

	//////함수//////
	void StartAiming(); // 조준
	void FireProjectile(); // 발사체 발사
	void DrawAimLine(); // 플레이어 - 보스까지의 일직선 레이저 그리기

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|Sniping")
	void SnipingPlayer(); // 위의 과정이 한번에

#pragma endregion


#pragma region BossCharacterSpecialAttacks_FireRandomShots
	//////변수//////
	// 총알 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	TSubclassOf<AActor> BulletClass;

	// 발사 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	int32 BulletCount = 40; // 총알 개수

	UPROPERTY(EditDefaultsOnly, Category = "SpecialAttack|FireRandomShots")
	float BulletSpreadAngle = 45.0f; // 부채꼴 범위 (도)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	float FireRate = 0.1f; // 연속 발사 간격

	int CurrentBulletCount;

	//여러발 사격시 사이 시간
	FTimerHandle ShootingDelayTimerHandle;

	// 타이머 핸들러 (여러 발 연속 발사 시 사용)
	FTimerHandle ShootingTimerHandle;

	//////함수//////
		// 총을 쏘는 함수
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|FireRandomShots")
	void FireRandomShots();

	// 총알 스폰용 함수
	void SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation);

	void StartFiring();

	// 공격 실행 함수
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|FireRandomShots")
	void StartRandomShooting();

	/////////////////////////////패턴시작 경고 표시/////////////////////////////

	//////변수//////

	//Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	UMaterialInterface* RandomShootingFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	UMaterialInterface* RandomShootingSecondMaterial;

	//////함수//////
	void ShowFireRandomShotsWarning();
#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnBigBomb
	//////변수//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnBigBomb")
	TSubclassOf<AActor> BigBombClass;

	//////함수//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnBigBomb")
	void SpawnBigBomb();

	/////////////////////////////패턴시작 경고 표시/////////////////////////////
	//////변수//////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnBigBomb")
	UMaterialInterface* BigBombFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnBigBomb")
	UMaterialInterface* BigBombSecondMaterial;

	//////함수//////
	void ShowBigBombWarning();
#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnSmallBombs
	//////변수//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	TSubclassOf<AActor> BombClass;

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	int32 BombCount = 10;  // 한 번에 떨어지는 폭탄 수

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	float BombDropInterval = 0.5f;  // 미사일이 떨어지는 간격

	FTimerHandle SpawnSmallBombDelayTimerHandle;

	//////함수//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnSmallBombs")
	void StartSpawnBombs();

	UFUNCTION()
	void SpawnBombs(FVector TargetLocation);

	/////////////////////////////패턴시작 경고 표시/////////////////////////////
	//////변수//////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnSmallBombs")
	UMaterialInterface* SmallBombFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnSmallBombs")
	UMaterialInterface* SmallBombSecondMaterial;

	//////함수//////
	void ShowSmallBombWarning();
#pragma endregion
};
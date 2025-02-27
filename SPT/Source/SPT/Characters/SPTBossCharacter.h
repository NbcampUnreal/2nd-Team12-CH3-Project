// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SPTBossCharacter.generated.h"

UCLASS()
class SPT_API ASPTBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASPTBossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int HP;

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	int GetHP();

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void SetHP(int Amount);

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void SpawnMissile();

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void StartMissileAttack();

	FTimerHandle MissileTimerHandle;

	// 미사일 발사 위치를 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FVector MissileSpawnOffset; // 미사일의 발사 위치 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	int32 MissileCount = 3; // 미사일 갯수 (블루프린트에서 설정 가능)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	TSubclassOf<AActor> MissileClass; // 미사일 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	float MissileSpeed = 600.0f; // 미사일 속도

	int CurrentMissileCount;


	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void SnipingPlayer();

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AimDuration = 2.0f; // 조준 지속 시간

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // 발사할 총알 클래스

	bool bIsAiming = false;  // 현재 조준 중인지 확인하는 변수
	FTimerHandle AimTimerHandle;

	void StartAiming();
	void FireProjectile();
	void DrawAimLine();


	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void StartSpawnBombs();
	
	UFUNCTION()
	void SpawnBombs(FVector TargetLocation);
	FTimerHandle SpawnSmallBombDelayTimerHandle;


	UPROPERTY(EditAnywhere, Category = "Bomb")
	TSubclassOf<AActor> BombClass;

	UPROPERTY(EditAnywhere, Category = "Bomb")
	FVector BombSpawnLocation;  // 미사일이 떨어질 시작 위치

	UPROPERTY(EditAnywhere, Category = "Bomb")
	int32 BombCount = 10;  // 한 번에 떨어지는 폭탄 수

	UPROPERTY(EditAnywhere, Category = "Bomb")
	float BombDropInterval = 0.5f;  // 미사일이 떨어지는 간격


	UPROPERTY(EditAnywhere, Category = "BigBomb")
	TSubclassOf<AActor> BigBombClass;

	UFUNCTION(BlueprintCallable, Category = "BigBomb")
	void SpawnBigBomb();


	// 총을 쏘는 함수
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void FireRandomShots();

	// 총알 스폰용 함수
	void SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation);

	// 타이머 핸들러 (여러 발 연속 발사 시 사용)
	FTimerHandle ShootingTimerHandle;

	// 총알 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> BulletClass;

	// 발사 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int32 BulletCount = 20; // 총알 개수

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float BulletSpreadAngle = 45.0f; // 부채꼴 범위 (도)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FireRate = 0.2f; // 연속 발사 간격

	// 공격 실행 함수
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void StartRandomShooting();

	int CurrentBulletCount;

	/** 폭발 범위를 나타내는 데칼 */
	UPROPERTY()
	UDecalComponent* WarningDecal;

	//Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* RandomShootingFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* RandomShootingSecondMaterial;

	void ShowFireRandomShotsWarning();
	FTimerHandle ShootingDelayTimerHandle;

	void StartFiring();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* BigBombFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* BigBombSecondMaterial;

	void ShowBigBombWarning();




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* SmallBombFirstMaterial;

	//시간에 따라 확장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* SmallBombSecondMaterial;

	void ShowSmallBombWarning();

};


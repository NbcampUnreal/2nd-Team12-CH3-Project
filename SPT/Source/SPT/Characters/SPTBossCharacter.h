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

	/** ���� ������ ��Ÿ���� ��Į */
	UPROPERTY()
	UDecalComponent* WarningDecal;

#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnMissiles
	//////����//////
	int CurrentMissileCount;

	FTimerHandle MissileTimerHandle; // �̻��� �߻� ���� ��Ʈ�� Ÿ�̸�

	// �̻��� �߻� ��ġ�� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	FVector MissileSpawnOffset; // �̻����� �߻� ��ġ ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	int32 MissileCount = 3; // �̻��� ���� (�������Ʈ���� ���� ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	TSubclassOf<AActor> MissileClass; // �̻��� Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnMissile")
	float MissileSpeed = 600.0f; // �̻��� �ӵ�

	//////�Լ�//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnMissile")
	void SpawnMissile();

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnMissile")
	void StartMissileAttack();

#pragma endregion


#pragma region BossCharacterSpecialAttacks_Sniping
	//////����//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	float AimDuration = 2.0f; // ���� ���� �ð�

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	TSubclassOf<AActor> ProjectileClass; // �߻��� �Ѿ� Ŭ����

	bool bIsAiming = false;  // ���� ���� ������ Ȯ���ϴ� ����

	FTimerHandle AimTimerHandle; //���� �� �߻������ ����ð� ��Ʈ�� Ÿ�̸�

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UStaticMeshComponent* AimLineMeshComponent;

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UStaticMesh* CylinderMesh;

	// ������ ��Ƽ����
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|Sniping")
	UMaterialInterface* RedMaterial;

	//////�Լ�//////
	void StartAiming(); // ����
	void FireProjectile(); // �߻�ü �߻�
	void DrawAimLine(); // �÷��̾� - ���������� ������ ������ �׸���

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|Sniping")
	void SnipingPlayer(); // ���� ������ �ѹ���

#pragma endregion


#pragma region BossCharacterSpecialAttacks_FireRandomShots
	//////����//////
	// �Ѿ� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	TSubclassOf<AActor> BulletClass;

	// �߻� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	int32 BulletCount = 40; // �Ѿ� ����

	UPROPERTY(EditDefaultsOnly, Category = "SpecialAttack|FireRandomShots")
	float BulletSpreadAngle = 45.0f; // ��ä�� ���� (��)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	float FireRate = 0.1f; // ���� �߻� ����

	int CurrentBulletCount;

	//������ ��ݽ� ���� �ð�
	FTimerHandle ShootingDelayTimerHandle;

	// Ÿ�̸� �ڵ鷯 (���� �� ���� �߻� �� ���)
	FTimerHandle ShootingTimerHandle;

	//////�Լ�//////
		// ���� ��� �Լ�
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|FireRandomShots")
	void FireRandomShots();

	// �Ѿ� ������ �Լ�
	void SpawnBullet(FVector SpawnLocation, FRotator SpawnRotation);

	void StartFiring();

	// ���� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|FireRandomShots")
	void StartRandomShooting();

	/////////////////////////////���Ͻ��� ��� ǥ��/////////////////////////////

	//////����//////

	//Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	UMaterialInterface* RandomShootingFirstMaterial;

	//�ð��� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|FireRandomShots")
	UMaterialInterface* RandomShootingSecondMaterial;

	//////�Լ�//////
	void ShowFireRandomShotsWarning();
#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnBigBomb
	//////����//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnBigBomb")
	TSubclassOf<AActor> BigBombClass;

	//////�Լ�//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnBigBomb")
	void SpawnBigBomb();

	/////////////////////////////���Ͻ��� ��� ǥ��/////////////////////////////
	//////����//////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnBigBomb")
	UMaterialInterface* BigBombFirstMaterial;

	//�ð��� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnBigBomb")
	UMaterialInterface* BigBombSecondMaterial;

	//////�Լ�//////
	void ShowBigBombWarning();
#pragma endregion


#pragma region BossCharacterSpecialAttacks_SpawnSmallBombs
	//////����//////
	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	TSubclassOf<AActor> BombClass;

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	int32 BombCount = 10;  // �� ���� �������� ��ź ��

	UPROPERTY(EditAnywhere, Category = "SpecialAttack|SpawnSmallBombs")
	float BombDropInterval = 0.5f;  // �̻����� �������� ����

	FTimerHandle SpawnSmallBombDelayTimerHandle;

	//////�Լ�//////
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack|SpawnSmallBombs")
	void StartSpawnBombs();

	UFUNCTION()
	void SpawnBombs(FVector TargetLocation);

	/////////////////////////////���Ͻ��� ��� ǥ��/////////////////////////////
	//////����//////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnSmallBombs")
	UMaterialInterface* SmallBombFirstMaterial;

	//�ð��� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack|SpawnSmallBombs")
	UMaterialInterface* SmallBombSecondMaterial;

	//////�Լ�//////
	void ShowSmallBombWarning();
#pragma endregion
};
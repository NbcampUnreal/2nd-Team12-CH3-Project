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


	// �̻��� �߻� ��ġ�� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FVector MissileSpawnOffset; // �̻����� �߻� ��ġ ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	int32 MissileCount = 3; // �̻��� ���� (�������Ʈ���� ���� ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	TSubclassOf<AActor> MissileClass; // �̻��� Ŭ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missiles")
	float MissileSpeed = 600.0f; // �̻��� �ӵ�



	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void SnipingPlayer();

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AimDuration = 2.0f; // ���� ���� �ð�

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass; // �߻��� �Ѿ� Ŭ����

	bool bIsAiming = false;  // ���� ���� ������ Ȯ���ϴ� ����
	FTimerHandle AimTimerHandle;

	void StartAiming();
	void FireProjectile();
	void DrawAimLine();


	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void SpawnBombs();

	UPROPERTY(EditAnywhere, Category = "Bomb")
	TSubclassOf<AActor> BombClass;

	UPROPERTY(EditAnywhere, Category = "Bomb")
	FVector BombSpawnLocation;  // �̻����� ������ ���� ��ġ

	UPROPERTY(EditAnywhere, Category = "Bomb")
	int32 BombCount = 10;  // �� ���� �������� ��ź ��

	UPROPERTY(EditAnywhere, Category = "Bomb")
	float BombDropInterval = 0.5f;  // �̻����� �������� ����
};

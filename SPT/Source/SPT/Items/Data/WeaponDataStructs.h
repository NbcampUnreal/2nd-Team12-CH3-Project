// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.generated.h"

/* ���� ���� Enum */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Firearm UMETA(DisplayName = "Firearm Weapon"),	// ���Ÿ� ����
	EWT_Melee UMETA(DisplayName = "Melee Weapon"),		// ���� ����
	EWT_Throwable UMETA(DisplayName = "Throwable Weapon"),	// ��ô ����
	EWT_MAX UMETA(Hidden)
};

/* ���� ���� �Ӽ� */
USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRate;	// ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;	// �⺻ ���ݷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;	// ���� ����
};

/* �ѱ� ������ ����ü */
USTRUCT(BlueprintType)
struct FFirearmStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 AmmoCount;	// ���� ź�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 MagazineCapacity;	// źâ ũ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float FireRate;		// ���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ReloadTime;	// ������ �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float BulletSpread;	// ź���� (���߷�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ProjectileSpeed;	// �Ѿ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float Recoil;	// �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	bool bHasAutomaticFireMode; // �ڵ� ��� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|ADS")
	float AimZoomLevel;	// ���� �� Ȯ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|ADS")
	float AimSpeed;	// ���� �ӵ�
};

/* ���� ���� ������ ����ü */
USTRUCT(BlueprintType)
struct FMeleeWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float SwingSpeed;	// �ֵθ��� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float AttackRange;	// ���� ����
};

/* ����ź ������ ����ü */
USTRUCT(BlueprintType)
struct FGrenadeStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float ExplosionRadius;	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float FuseTime;		// ���߱��� �ҿ� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float ThrowForce;	// ��ô��
};

/* ���� ������ ������ ����ü */
USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/* ���� ���� (�ѱ�, ���� ����, ����ź ��) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Data")
	EWeaponType WeaponType;

	/* ���� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	FWeaponStats WeaponStats;

	/* �ѱ� ������ (�ѱ��� ��쿡�� �Ҵ�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm", meta = (EditCondition = "WeaponType == EWT_Firearm"))
	FFirearmStats FirearmStats;

	/* ���� ���� ������ (���� ������ ��쿡�� �Ҵ�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee", meta = (EditCondition = "WeaponType == EWT_Melee"))
	FMeleeWeaponStats MeleeStats;

	/* ����ź ������ (����ź�� ��쿡�� �Ҵ�) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Throwable", meta = (EditCondition = "WeaponType == EWT_Grenade"))
	FGrenadeStats GrenadeStats;

	/* ���� ���� Physics Asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Assets")
	UPhysicsAsset* PhysicsAsset;
};

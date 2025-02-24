// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.generated.h"

/* ���� ���� Enum */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Firearm UMETA(DisplayName = "Firearm"),      // �ѱ��
	EWT_Melee UMETA(DisplayName = "MeleeWeapon"),   // ���� ����
	EWT_Grenade UMETA(DisplayName = "Grenade"),     // ����ź
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
	float BulletSpread;	// ź����(���߷�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ProjectileSpeed;	// �Ѿ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float Recoil;	// �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	bool bHasAutomaticFireMode; // �ڵ� ��� ���� ����
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ExplosionRadius;	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float FuseTime;		// ���߱��� �ҿ� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ThrowForce;	// ��ô��
};

/* ���� ������ ������ ����ü */
USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Data")
	EWeaponType WeaponType;  // ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	FWeaponStats WeaponStats;  // ���� ���� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm", meta = (EditCondition = "WeaponType == EWT_Firearm"))
	FFirearmStats FirearmStats;  // �ѱ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee", meta = (EditCondition = "WeaponType == EWT_Melee"))
	FMeleeWeaponStats MeleeStats;  // ���� ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Grenade", meta = (EditCondition = "WeaponType == EWT_Grenade"))
	FGrenadeStats GrenadeStats;  // ����ź ������

	/* ���� ���� Physics Asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Assets")
	UPhysicsAsset* PhysicsAsset;
};

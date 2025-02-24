// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.generated.h"

/* 무기 유형 Enum */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Firearm UMETA(DisplayName = "Firearm"),      // 총기류
	EWT_Melee UMETA(DisplayName = "MeleeWeapon"),   // 근접 무기
	EWT_Grenade UMETA(DisplayName = "Grenade"),     // 수류탄
	EWT_MAX UMETA(Hidden)
};

/* 무기 공통 속성 */
USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRate;	// 공격 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;	// 기본 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Range;	// 공격 범위
};

/* 총기 데이터 구조체 */
USTRUCT(BlueprintType)
struct FFirearmStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 AmmoCount;	// 현재 탄약 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 MagazineCapacity;	// 탄창 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float FireRate;		// 연사 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ReloadTime;	// 재장전 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float BulletSpread;	// 탄착군(명중률)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ProjectileSpeed;	// 총알 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float Recoil;	// 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	bool bHasAutomaticFireMode; // 자동 사격 가능 여부
};

/* 근접 무기 데이터 구조체 */
USTRUCT(BlueprintType)
struct FMeleeWeaponStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float SwingSpeed;	// 휘두르는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeWeapon")
	float AttackRange;	// 공격 범위
};

/* 수류탄 데이터 구조체 */
USTRUCT(BlueprintType)
struct FGrenadeStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ExplosionRadius;	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float FuseTime;		// 폭발까지 소요 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float ThrowForce;	// 투척력
};

/* 무기 아이템 데이터 구조체 */
USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Data")
	EWeaponType WeaponType;  // 무기 유형
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	FWeaponStats WeaponStats;  // 공통 무기 데이터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm", meta = (EditCondition = "WeaponType == EWT_Firearm"))
	FFirearmStats FirearmStats;  // 총기 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee", meta = (EditCondition = "WeaponType == EWT_Melee"))
	FMeleeWeaponStats MeleeStats;  // 근접 무기 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Grenade", meta = (EditCondition = "WeaponType == EWT_Grenade"))
	FGrenadeStats GrenadeStats;  // 수류탄 데이터

	/* 무기 전용 Physics Asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Assets")
	UPhysicsAsset* PhysicsAsset;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataStructs.generated.h"
/////////////////////////////////////////////////
class AWeaponBase;
/////////////////////////////////////////////////
/* 무기 유형 Enum */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Firearm UMETA(DisplayName = "Firearm Weapon"),	// 원거리 무기
	EWT_Melee UMETA(DisplayName = "Melee Weapon"),		// 근접 무기
	EWT_Throwable UMETA(DisplayName = "Throwable Weapon"),	// 투척 무기
	EWT_MAX UMETA(Hidden)
};

/* 총기 종류 Enum */
UENUM(BlueprintType)
enum class EFirearmType : uint8
{
	EFT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EFT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun"),
	EFT_Pistol UMETA(DisplayName = "Pistol"),
	EFT_MAX UMETA(Hidden)
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
};

/* 총기 에임 */
USTRUCT()
struct FWeaponAimData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength;
	UPROPERTY(EditAnywhere)
	FVector SocketOffset;
	UPROPERTY(EditAnywhere)
	float FieldOfView;
	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;

public:
	void SetData(class ASPTPlayerCharacter* InOwner);
	void SetDataByNoneCurve(class ASPTPlayerCharacter* InOwner);
};

/* 총기 데이터 구조체 */
USTRUCT(BlueprintType)
struct FFirearmStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	EFirearmType FirearmType;	// 총기 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 AmmoCount;	// 현재 탄약 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	int32 MagazineCapacity;	// 탄창 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float FireRate;		// 연사 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ReloadTime;	// 재장전 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float HitDistance;	// 공격 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	float ProjectileSpeed;	// 총알 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm")
	bool bHasAutomaticFireMode; // 자동 사격 가능 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	FName HolsterSocketName;	// 장착 해제 시 소켓
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	float EquipMontage_PlayRate;	// 장착에 걸리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	FName RightHandSocketName;	// 장착 소켓
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	FVector LeftHandLocation;	// 왼손 위치

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Aim")
	float AimZoomLevel;	// 조준 시 확대 비율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Aim")
	float AimSpeed;	// 조준 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Aim")
	class UCurveFloat* AimCurve;	// 조준 커브
	UPROPERTY(EditAnywhere, Category = "Firearm|Aim")
	FWeaponAimData BaseData;
	UPROPERTY(EditAnywhere, Category = "Firearm|Aim")
	FWeaponAimData AimData;
	UPROPERTY(EditAnywhere, Category = "Firearm|Aim")
	class UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	class UMaterialInterface* HitDecal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	class UParticleSystem* HitParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	class UParticleSystem* FlashParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	TSubclassOf<AActor> EjectShellActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	class USoundWave* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	float RecoilAngle;	// 탄착군 Cone 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Fire")
	float RecoilRate;	// 반동률

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	class UAnimMontage* FireMontage;	// 발사 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	class UAnimMontage* EquipMontage;	// 장착 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Firearm|Equip")
	class UAnimMontage* ReloadMontage;	// 재장전 몽타주

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float ExplosionRadius;	// 폭발 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float FuseTime;		// 폭발까지 소요 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwable")
	float ThrowForce;	// 투척력
};

/* 무기 아이템 데이터 구조체 */
USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	/* 무기 유형 (총기, 근접 무기, 수류탄 등) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Data")
	EWeaponType WeaponType;

	/* 공통 무기 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	FWeaponStats WeaponStats;

	/* 총기 데이터 (총기일 경우에만 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm", meta = (EditCondition = "WeaponType == EWT_Firearm"))
	FFirearmStats FirearmStats;

	/* 근접 무기 데이터 (근접 무기일 경우에만 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Melee", meta = (EditCondition = "WeaponType == EWT_Melee"))
	FMeleeWeaponStats MeleeStats;

	/* 수류탄 데이터 (수류탄일 경우에만 할당) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Throwable", meta = (EditCondition = "WeaponType == EWT_Grenade"))
	FGrenadeStats GrenadeStats;

	/* 무기 전용 Physics Asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Assets")
	UPhysicsAsset* PhysicsAsset;

	/////////////////////////////////////////////////
	// 인벤토리에서 무기 생성을 관리하기 위해 추가하였습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Weapon")
	TSubclassOf<AWeaponBase> WeaponClass;
	/////////////////////////////////////////////////
};

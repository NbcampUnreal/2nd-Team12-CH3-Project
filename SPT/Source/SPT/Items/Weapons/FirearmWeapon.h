// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "FirearmWeapon.generated.h"

UCLASS()
class SPT_API AFirearmWeapon : public AWeaponBase
{
	GENERATED_BODY()
	

public:
	AFirearmWeapon();

	/* 기본 총기 공격 */
	virtual void Attack() override;

	/* 무기 장착 기능 */
	virtual void Equip(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 무기 해제 기능 */
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 무기 드롭 기능 */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 재장전 */
	void Reload();

	/* 현재 탄약 개수 반환 */
	int32 GetCurrentAmmo() const;

	/* 조준 기능 */
	void AimDownSights();

	/* 조준 해제 */
	void StopAiming();

protected:

	/* 총기 전용 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm")
	FFirearmStats FirearmStats;

	/* 개별 무기가 유지해야 하는 속성 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	int32 CurrentAmmo;  // 현재 탄약 개수

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsReloading;  // 현재 재장전 중인지 여부

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsFiring;  // 현재 사격 중인지 여부

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsAiming; // 조준 여부

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	float CurrentRecoil;  // 현재 반동 값 (누적 반동)

	/* SkeletalMeshComponent (총기, 투척 무기 전용) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/* 총기 데이터를 불러와 초기화 */
	void InitializeFirearmData(const FWeaponItemData& NewWeaponData);

};

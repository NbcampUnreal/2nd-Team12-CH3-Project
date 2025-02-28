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

	virtual void BeginPlay() override;

	/* 기본 총기 공격 */
	virtual void Attack() override;
	bool CanFire();
	void Begin_Fire();
	void End_Fire();
	UFUNCTION()
	void OnFiring();

	/* 무기 장착 */
	bool CanEquip();
	virtual void Equip(ASPTPlayerCharacter* PlayerCharacter) override;
	void Begin_Equip();
	void End_Equip();

	/* 무기 해제 */
	bool CanUnEquip();
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* 무기 드롭 기능 */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* 재장전 */
	bool CanRelad();
	void Begin_Reload();
	void Reload();
	void End_Reload();

	/* 조준 기능 */
	bool bIsInAim();
	bool CanAim();
	void AimDownSights();
	void StopAiming();

	/* 현재 탄약 개수 반환 */
	int32 GetCurrentAmmo() const;


protected:

	/* 총기 전용 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm")
	FFirearmStats FirearmStats;

	/* 개별 무기가 유지해야 하는 속성 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsEquipping;  // 현재 장착 진행 중인지 여부

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
	virtual void SetWeaponData(const FWeaponItemData& NewWeaponData) override;

	void UpdateMesh();

public:
	class ASPTPlayerCharacter* Owner;
	FTimerHandle FireTimerHandle;

};

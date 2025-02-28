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

	/* �⺻ �ѱ� ���� */
	virtual void Attack() override;
	bool CanFire();
	void Begin_Fire();
	void End_Fire();
	UFUNCTION()
	void OnFiring();

	/* ���� ���� */
	bool CanEquip();
	virtual void Equip(ASPTPlayerCharacter* PlayerCharacter) override;
	void Begin_Equip();
	void End_Equip();

	/* ���� ���� */
	bool CanUnEquip();
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* ���� ��� ��� */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ������ */
	bool CanRelad();
	void Begin_Reload();
	void Reload();
	void End_Reload();

	/* ���� ��� */
	bool bIsInAim();
	bool CanAim();
	void AimDownSights();
	void StopAiming();

	/* ���� ź�� ���� ��ȯ */
	int32 GetCurrentAmmo() const;


protected:

	/* �ѱ� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm")
	FFirearmStats FirearmStats;

	/* ���� ���Ⱑ �����ؾ� �ϴ� �Ӽ� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsEquipping;  // ���� ���� ���� ������ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsReloading;  // ���� ������ ������ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsFiring;  // ���� ��� ������ ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	bool bIsAiming; // ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	float CurrentRecoil;  // ���� �ݵ� �� (���� �ݵ�)

	/* SkeletalMeshComponent (�ѱ�, ��ô ���� ����) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* SkeletalMeshComponent;

	/* �ѱ� �����͸� �ҷ��� �ʱ�ȭ */
	virtual void SetWeaponData(const FWeaponItemData& NewWeaponData) override;

	void UpdateMesh();

public:
	class ASPTPlayerCharacter* Owner;
	FTimerHandle FireTimerHandle;

};

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

	/* �⺻ �ѱ� ���� */
	virtual void Attack() override;

	/* ���� ���� ��� */
	virtual void Equip(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ���� ���� ��� */
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ���� ��� ��� */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ������ */
	void Reload();

	/* ���� ź�� ���� ��ȯ */
	int32 GetCurrentAmmo() const;

	/* ���� ��� */
	void AimDownSights();

	/* ���� ���� */
	void StopAiming();

protected:

	/* �ѱ� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Firearm")
	FFirearmStats FirearmStats;

	/* ���� ���Ⱑ �����ؾ� �ϴ� �Ӽ� */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Firearm")
	int32 CurrentAmmo;  // ���� ź�� ����

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
	void InitializeFirearmData(const FWeaponItemData& NewWeaponData);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "SPT/Interfaces/WeaponItemInterface.h"
#include "WeaponBase.generated.h"

UCLASS()
class SPT_API AWeaponBase : public AItemBase, public IWeaponItemInterface
{
	GENERATED_BODY()

public:

	AWeaponBase();

public:

	virtual void PrimaryAction(class ASPTPlayerCharacter* PlayerCharacter) override;

	/* ���� ��� */
	virtual void Attack() PURE_VIRTUAL(AWeaponBase::Attack, );

	/* ���� ���� ��� */
	virtual void Equip() PURE_VIRTUAL(AWeaponBase::Equip, );

	/* ���� ���� ��� */
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) PURE_VIRTUAL(AWeaponBase::UnEquip, );

	/* ���� ��� ��� */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ���� ������ ��ȯ */
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	virtual const FWeaponItemData& GetWeaponData() const;
	UFUNCTION(BlueprintCallable, Category = "Item|Weapons")
	virtual void SetWeaponData(const FWeaponItemData& NewWeaponData);

protected:
	/* ���� ���� ������ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponItemData WeaponData;

	void GetDropLocation(
		ASPTPlayerCharacter* PlayerCharacter, 
		FVector& OutDropLocation, 
		FRotator& OutDropRotation);
};

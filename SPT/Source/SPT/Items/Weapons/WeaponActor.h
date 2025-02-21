// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/EquipableItem.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "WeaponActor.generated.h"

UCLASS()
class SPT_API AWeaponActor : public AEquipableItem
{
	GENERATED_BODY()

public:
	AWeaponActor();

	virtual void Attack() PURE_VIRTUAL(AWeaponActor::Attack, );

	virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter) override;
	virtual bool UnEquip(ASPTPlayerCharacter* PlayerCharacter) override;
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) override;

	/* ���� ������ �������� */
	EWeaponType GetWeaponType() const;
	FWeaponItemData GetWeaponData() const;

protected:
	/* ���� �⺻ ������ */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponItemData WeaponData;

	/* ���� ���� ���� */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EItemState WeaponState;
	
};

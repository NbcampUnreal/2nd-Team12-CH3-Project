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

	/* 공격 기능 */
	virtual void Attack() PURE_VIRTUAL(AWeaponBase::Attack, );

	/* 무기 장착 기능 */
	virtual void Equip() override;

	/* 무기 해제 기능 */
	virtual void UnEquip() override;

	/* 무기 드롭 기능 */
	virtual void Drop() override;

	/* 무기 변경 */
	virtual void SwitchWeapon(AWeaponBase* NewWeapon);

	/* 무기 데이터 반환 */
	virtual const FWeaponItemData& GetWeaponData() const;

};

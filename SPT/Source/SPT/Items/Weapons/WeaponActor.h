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

	/* 무기 데이터 가져오기 */
	EWeaponType GetWeaponType() const;
	FWeaponItemData GetWeaponData() const;

protected:
	/* 무기 기본 데이터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponItemData WeaponData;

	/* 현재 무기 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	EItemState WeaponState;
	
};

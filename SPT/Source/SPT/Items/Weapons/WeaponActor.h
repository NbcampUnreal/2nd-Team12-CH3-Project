// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"
#include "SPT/Items/Data/WeaponDataStructs.h"
#include "WeaponActor.generated.h"

UCLASS()
class SPT_API AWeaponActor : public AWorldItemActor
{
	GENERATED_BODY()

public:
	AWeaponActor();

	/* 공격 기능 (순수 가상 함수) */
	virtual void Attack() PURE_VIRTUAL(AWeaponActor::Attack, );

	/* 아이템 줍기 */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;
	
	/* 아이템 드랍 */
	virtual void OnDrop(ASPTPlayerCharacter* PlayerCharacter);

	/* 무기 데이터 가져오기 */
	EWeaponType GetWeaponType() const;
	FWeaponItemData GetWeaponData() const;
};

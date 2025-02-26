// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponItemInterface.generated.h"

class ASPTPlayerCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SPT_API IWeaponItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* 공격 기능 */
	virtual void Attack() = 0;

	/* 무기 장착 기능 */
	virtual void Equip(ASPTPlayerCharacter* PlayerCharacter) = 0;

	/* 무기 해제 기능 */
	virtual void UnEquip(ASPTPlayerCharacter* PlayerCharacter) = 0;

	/* 무기 드롭 기능 */
	virtual void Drop(ASPTPlayerCharacter* PlayerCharacter) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipableInterface.generated.h"

class ASPTPlayerCharacter;

UINTERFACE(MinimalAPI)
class UEquipableInterface : public UInterface
{
	GENERATED_BODY()
};

class SPT_API IEquipableInterface
{
	GENERATED_BODY()

public:
	// 장착 가능한지 체크하는 함수
	virtual bool CanEquip(ASPTPlayerCharacter* PlayerCharacter) const = 0;

	// 아이템 장착 및 해제 함수
	virtual bool Equip(ASPTPlayerCharacter* PlayerCharacter) = 0;
	virtual bool UnEquip(ASPTPlayerCharacter* PlayerCharacter) = 0;
};

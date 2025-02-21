// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Base/EquipableItem.h"
#include "ConsumableItemActor.generated.h"

UCLASS()
class SPT_API AConsumableItemActor : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AConsumableItemActor();

	/* ������ ��� */
	virtual void Use(ASPTPlayerCharacter* PlayerCharacter);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPT/Items/Worlds/WorldItemBase.h"
#include "WorldConsumable.generated.h"

UCLASS()
class SPT_API AWorldConsumable : public AWorldItemBase
{
	GENERATED_BODY()

public:

	AWorldConsumable();

	/* 아이템 줍는 기능 */
	virtual void OnPickup(ASPTPlayerCharacter* PlayerCharacter) override;
	
};

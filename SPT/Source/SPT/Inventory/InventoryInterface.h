// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UInventoryItem;

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};


class SPT_API IInventoryInterface
{
	GENERATED_BODY()

public:
	virtual void AddItem(UInventoryItem* Item) = 0;
	virtual void DropItem(UInventoryItem* Item) = 0;
	virtual bool HasItem(FName ItemName) = 0;
	virtual bool RemoveItem(UInventoryItem* Item) = 0;
	virtual const TArray<UInventoryItem*>& GetInventory() const = 0;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableInventory.h"

AConsumableInventory::AConsumableInventory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConsumableInventory::AddItem(UInventoryItem* Item)
{
	if (UConsumableItemDataObject* Consumable = Cast<UConsumableItemDataObject>(Item))
	{
		ConsumableItems.Add(Consumable);
	}
}

void AConsumableInventory::DropItem(UInventoryItem* Item)
{
	ConsumableItems.Remove(Cast<UConsumableItemDataObject>(Item));
}

bool AConsumableInventory::HasItem(FName ItemName)
{
	return ConsumableItems.ContainsByPredicate([ItemName](UConsumableItemDataObject* Item)
		{
			return Item->ItemName == ItemName;
		});
}

bool AConsumableInventory::RemoveItem(UInventoryItem* Item)
{
	if (!Item) return false;

	if (ConsumableItems.Remove(Cast<UConsumableItemDataObject>(Item)) > 0)
	{
		return true;
	}

	return false;
}

const TArray<UInventoryItem*>& AConsumableInventory::GetInventory() const
{
	return reinterpret_cast<const TArray<UInventoryItem*>&>(ConsumableItems);
}

void AConsumableInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConsumableInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


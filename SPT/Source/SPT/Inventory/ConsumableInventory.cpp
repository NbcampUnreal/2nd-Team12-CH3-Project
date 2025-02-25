// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableInventory.h"

AConsumableInventory::AConsumableInventory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConsumableInventory::AddItem(UInventoryItem* Item)
{
	if (UConsumableItem* Consumable = Cast<UConsumableItem>(Item))
	{
		ConsumableItems.Add(Consumable);
	}
}

void AConsumableInventory::DropItem(UInventoryItem* Item)
{
	ConsumableItems.Remove(Cast<UConsumableItem>(Item));
}

bool AConsumableInventory::HasItem(FName ItemName)
{
	return ConsumableItems.ContainsByPredicate([ItemName](UConsumableItem* Item)
		{
			return Item->ItemName == ItemName;
		});
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


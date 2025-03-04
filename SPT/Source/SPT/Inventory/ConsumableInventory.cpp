// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableInventory.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"

AConsumableInventory::AConsumableInventory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConsumableInventory::AddItem(UInventoryItem* Item)
{
    // 아이템이 소비 아이템인 경우 ConsumableItems에 추가
    if (Item && Item->GetItemBaseClass())
    {
        if (Item->IsConsumable())  // 소비 아이템 체크
        {
            ConsumableItems.Add(Item);
        }
        else if (Item->IsWeapon())  // 무기 아이템인 경우 다른 배열에 추가
        {
            UE_LOG(LogTemp, Warning, TEXT("Item is a weapon, cannot be added to ConsumableInventory"));
        }
    }
}

void AConsumableInventory::DropItem(UInventoryItem* Item)
{
	ConsumableItems.Remove(Cast<UInventoryItem>(Item));
}

bool AConsumableInventory::HasItem(FName ItemName)
{
	return ConsumableItems.ContainsByPredicate([ItemName](UInventoryItem* Item)
		{
			return FName(*Item->GetItemName().ToString()) == ItemName;
		});
}

bool AConsumableInventory::RemoveItem(UInventoryItem* Item)
{
	if (!Item) return false;

	if (ConsumableItems.Remove(Cast<UInventoryItem>(Item)) > 0)
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


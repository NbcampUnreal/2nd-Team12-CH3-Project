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
    // �������� �Һ� �������� ��� ConsumableItems�� �߰�
    if (Item && Item->GetItemBaseClass())
    {
        if (Item->IsConsumable())  // �Һ� ������ üũ
        {
            ConsumableItems.Add(Item);
        }
        else if (Item->IsWeapon())  // ���� �������� ��� �ٸ� �迭�� �߰�
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


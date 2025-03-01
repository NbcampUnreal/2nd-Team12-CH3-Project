// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentInventory.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"

AEquipmentInventory::AEquipmentInventory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEquipmentInventory::AddItem(UInventoryItem* Item)
{
    // �������� ������ ��� EquipmentItems�� �߰�
    if (Item && Item->GetItemBaseClass())
    {
        if (Item->IsWeapon())  // ���� ������ üũ
		{
			// ���� �������̶�� EquipmentItems �迭�� �߰�
			EquipmentItems.Add(Item);
		}
        else if (Item->IsConsumable())  // �Һ� �������� ��� �ٸ� �迭�� �߰�
        {
            UE_LOG(LogTemp, Warning, TEXT("Item is consumable, cannot be added to EquipmentInventory"));
        }
    }
}

void AEquipmentInventory::DropItem(UInventoryItem* Item)
{
	EquipmentItems.Remove(Cast<UInventoryItem>(Item));
}

bool AEquipmentInventory::HasItem(FName ItemName)
{
	return EquipmentItems.ContainsByPredicate([ItemName](UInventoryItem* Item)
	{
		return FName(*Item->GetItemName().ToString()) == ItemName;
	});
}

bool AEquipmentInventory::RemoveItem(UInventoryItem* Item)
{
	if (!Item) return false;

	if (EquipmentItems.Remove(Cast<UInventoryItem>(Item)) > 0)
	{
		return true;
	}

	return false;
}

const TArray<UInventoryItem*>& AEquipmentInventory::GetInventory() const
{
	return reinterpret_cast<const TArray<UInventoryItem*>&>(EquipmentItems);
}

void AEquipmentInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipmentInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


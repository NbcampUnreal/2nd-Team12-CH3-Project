// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotInventory.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"

// Sets default values
AEquipmentSlotInventory::AEquipmentSlotInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEquipmentSlotInventory::AddItem(UInventoryItem* Item)
{
    // GetSlotType�� -1�� ���� ����Ʈ ���� ����(��� �ƴ� ���)
    if (Item && Item->GetSlotType() != -1)
    {
        // �������� ���� ������ ������ ������ ����
        EquipItem(Item, Item->GetSlotType());
    }
}

void AEquipmentSlotInventory::DropItem(UInventoryItem* Item)
{
    if (Item)
    {
        // �ش� �������� ���� ����
        UnequipItem(Item->GetSlotType());
    }
}

bool AEquipmentSlotInventory::HasItem(FName ItemName)
{
    // ���⼭ Slot�� UInventoryItem Ÿ���� ���� ���Ѵ�.
    for (auto& Slot : EquippedItems)
    {
        if (Slot.Value->GetItemName().ToString() == ItemName)
        {
            return true;
        }
    }
    return false;
}

bool AEquipmentSlotInventory::RemoveItem(UInventoryItem* Item)
{
    if (!Item) return false;

    // �ش� �������� ������ ������ ã�Ƽ� ����
    UnequipItem(Item->GetSlotType());
    return true;
}

const TArray<UInventoryItem*>& AEquipmentSlotInventory::GetInventory() const
{
    // ������ �����۵鸸 ��ȯ (�ʿ�� ó��)
    static TArray<UInventoryItem*> InventoryItems;
    InventoryItems.Empty();
    for (const auto& Slot : EquippedItems)
    {
        InventoryItems.Add(Slot.Value);
    }
    return InventoryItems;
}

bool AEquipmentSlotInventory::EquipItem(UInventoryItem* Item, int32 Slot)
{
    if (!Item) return false;

    // ���� ��� ����
    if (EquippedItems.Contains(Slot))
    {
        UnequipItem(Slot);
    }

    // ���ο� ������ ����
    EquippedItems.Add(Slot, Item);
    return true;
}

UInventoryItem* AEquipmentSlotInventory::UnequipItem(int32 Slot)
{
    UInventoryItem* UnequippedItem = nullptr;
    if (EquippedItems.Contains(Slot))
    {
        UnequippedItem = EquippedItems[Slot];
        EquippedItems.Remove(Slot);
    }
    return UnequippedItem;
}

UInventoryItem* AEquipmentSlotInventory::IsEquippedSlot(UInventoryItem* Item)
{
    if (!Item) return nullptr;

    int32 Slot = Item->GetSlotType();

    // �ش� ���Կ� �̹� ������ �������� �ִ��� Ȯ��
    if (EquippedItems.Contains(Slot))
    {
        return EquippedItems[Slot];
    }
    return nullptr;
}

// Called when the game starts or when spawned
void AEquipmentSlotInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEquipmentSlotInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


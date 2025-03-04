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
    // GetSlotType이 -1인 경우는 디폴트 값을 말함(장비가 아닌 경우)
    if (Item && Item->GetSlotType() != -1)
    {
        // 아이템이 장착 가능한 슬롯이 있으면 장착
        EquipItem(Item, Item->GetSlotType());
    }
}

void AEquipmentSlotInventory::DropItem(UInventoryItem* Item)
{
    if (Item)
    {
        // 해당 아이템을 장착 해제
        UnequipItem(Item->GetSlotType());
    }
}

bool AEquipmentSlotInventory::HasItem(FName ItemName)
{
    // 여기서 Slot은 UInventoryItem 타입의 값을 말한다.
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

    // 해당 아이템이 장착된 슬롯을 찾아서 제거
    UnequipItem(Item->GetSlotType());
    return true;
}

const TArray<UInventoryItem*>& AEquipmentSlotInventory::GetInventory() const
{
    // 장착된 아이템들만 반환 (필요시 처리)
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

    // 기존 장비 해제
    if (EquippedItems.Contains(Slot))
    {
        UnequipItem(Slot);
    }

    // 새로운 아이템 장착
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

    // 해당 슬롯에 이미 장착된 아이템이 있는지 확인
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


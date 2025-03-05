// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentPanelWidget.h"
#include "SPT/Inventory/EquipmentSlotInventory.h"
#include "SPT/Inventory/ItemWidget/EquipmentSlotWidget.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"

void UEquipmentPanelWidget::UpdateEquipmentSlots(AEquipmentSlotInventory* EquipmentSlotInventory)
{
    if (!EquipmentSlotInventory) return;

    TArray<UInventoryItem*> EquippedItems = EquipmentSlotInventory->GetInventory();

    for (UInventoryItem* Item : EquippedItems)
    {
        switch (Item->GetSlotType())
        {
        case 0:
            WeaponSlot->UpdateSlot(Item);
            break;
        case 1:
            BodySlot->UpdateSlot(Item);
            break;
        case 2:
            HeadSlot->UpdateSlot(Item);
            break;
        }
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UEquipmentSlotWidget::UpdateSlot(UInventoryItem* EquippedItem)
{
    if (EquippedItem)
    {
        ItemIcon->SetBrushFromTexture(EquippedItem->GetItemIcon());
        ItemName->SetText(EquippedItem->GetItemName());
    }
    else
    {
        ItemIcon->SetBrushFromTexture(nullptr);
        ItemName->SetText(FText::FromString("Empty"));
    }
}

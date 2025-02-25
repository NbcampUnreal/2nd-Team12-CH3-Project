// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UInventoryItem* InventoryItem = Cast<UInventoryItem>(ListItemObject);
	if (InventoryItem)
	{
		UpdateItem(InventoryItem);
	}
}

void UInventorySlotWidget::UpdateItem(UInventoryItem* InventoryItem)
{
	if (!InventoryItem) return;

	ItemNameText->SetText(FText::FromString(InventoryItem->ItemName));
	ItemIconImage->SetBrushFromTexture(InventoryItem->ItemIcon);
}

void UInventorySlotWidget::OnItemSelectClicked()
{
}

void UInventorySlotWidget::OnUseClicked()
{
}

void UInventorySlotWidget::OnDropClicked()
{
}

void UInventorySlotWidget::OnCancelClicked()
{
}

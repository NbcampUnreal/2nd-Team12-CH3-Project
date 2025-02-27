// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "SPT/Inventory/ItemData/InventoryItem.h"
#include "InventoryManager.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "SPTPlayerCharacter.h"

void UInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    UE_LOG(LogTemp, Warning, TEXT("NativeOnListItemObjectSet called"));

    if (!ListItemObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("ListItemObject is nullptr"));
        return;
    }

	UInventoryItem* InventoryItem = Cast<UInventoryItem>(ListItemObject);
	if (InventoryItem)
	{
        // 아이템 데이터 확인용(없어도됨)
        ItemData = InventoryItem;
        UE_LOG(LogTemp, Warning, TEXT("Slot Assigned Item : % s"), *ItemData->GetItemName());


		UpdateItem(InventoryItem);
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemData is nullptr"));
    }

    if (ItemSelectButton)
    {
        ItemSelectButton->OnClicked.RemoveAll(this);
        ItemSelectButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnItemSelectClicked);
    }

    if (UseButton)
    {
        UseButton->OnClicked.RemoveAll(this);
        UseButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnUseClicked);

    }
    if (DropButton)
    {
        DropButton->OnClicked.RemoveAll(this);
        DropButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnDropClicked);

    }
    if (CancelButton)
    {
        CancelButton->OnClicked.RemoveAll(this);
        CancelButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnCancelClicked);
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
    UseButton->SetVisibility(ESlateVisibility::Visible);
    DropButton->SetVisibility(ESlateVisibility::Visible);
    CancelButton->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlotWidget::OnUseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnUseClicked"));
    
    if (ItemData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Use %s"), *ItemData->GetItemName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Item Data Found!"));
    }
    
    ASPTPlayerCharacter* PlayerCharacter = Cast<ASPTPlayerCharacter>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        AInventoryManager* InventoryManager = PlayerCharacter->GetInventory();
        InventoryManager->UseItem(ItemData);
    }

    UseButton->SetVisibility(ESlateVisibility::Hidden);
    DropButton->SetVisibility(ESlateVisibility::Hidden);
    CancelButton->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::OnDropClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("OnDropClicked"));
    if (ItemData)
    {
        ASPTPlayerCharacter* PlayerCharacter = Cast<ASPTPlayerCharacter>(GetOwningPlayerPawn());
        if (PlayerCharacter)
        {
            PlayerCharacter->DropItem(ItemData);
        }
    }
    UseButton->SetVisibility(ESlateVisibility::Hidden);
    DropButton->SetVisibility(ESlateVisibility::Hidden);
    CancelButton->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlotWidget::OnCancelClicked()
{
    UseButton->SetVisibility(ESlateVisibility::Hidden);
    DropButton->SetVisibility(ESlateVisibility::Hidden);
    CancelButton->SetVisibility(ESlateVisibility::Hidden);
}

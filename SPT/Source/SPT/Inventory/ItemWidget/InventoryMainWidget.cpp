// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMainWidget.h"
#include "InventorySlotWidget.h"
#include "InventoryItem.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

void UInventoryMainWidget::UpdateInventoryList(const TArray<UInventoryItem*>& Items)
{
	if (!InventoryListView) {
		UE_LOG(LogTemp, Error, TEXT("InventoryListView is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UpdateInventoryList called! Items count: %d"), Items.Num());

	InventoryListView->ClearListItems();

	for (UInventoryItem* Item : Items)
	{
		InventoryListView->AddItem(Item);
	}
}

void UInventoryMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

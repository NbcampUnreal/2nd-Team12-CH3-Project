// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "InventoryMainWidget.h"

// Sets default values
AInventoryManager::AInventoryManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInventoryManager::RegisterInventory(AActor* InventoryActor)
{
	if (InventoryActor && InventoryActor->Implements<UInventoryInterface>())
	{
		Inventories.Add(InventoryActor);
	}
}

void AInventoryManager::AddItemToInventory(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem: Item is nullptr!"));
        return;
    }

    bool bItemAdded = false;

    // 장비 아이템 추가
    if (UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(Item))
    {
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AEquipmentInventory* EquipmentInventory = Cast<AEquipmentInventory>(Inventory.GetObject());
            if (EquipmentInventory)
            {
                EquipmentInventory->AddItem(EquipmentItem);  // 장비 아이템 추가
                UE_LOG(LogTemp, Warning, TEXT("Added to EquipmentInventory"));
                bItemAdded = true;
                break;  // 첫 번째 장비 인벤토리만 추가
            }
        }
    }

    // 소모품 아이템 추가
    if (!bItemAdded && Cast<UConsumableItem>(Item))
    {
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AConsumableInventory* ConsumableInventory = Cast<AConsumableInventory>(Inventory.GetObject());
            if (ConsumableInventory)
            {
                ConsumableInventory->AddItem(Cast<UConsumableItem>(Item));  // 소모품 아이템 추가
                UE_LOG(LogTemp, Warning, TEXT("Added to ConsumableInventory"));
                bItemAdded = true;
                break;  // 첫 번째 소모품 인벤토리만 추가
            }
        }
    }

    // 아이템이 추가되었으면 UI 갱신
    if (bItemAdded && InventoryMainWidgetInstance)
    {
        // 모든 인벤토리에서 아이템 목록을 가져와 병합
        TArray<UInventoryItem*> AllItems;

        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            if (AEquipmentInventory* EquipmentInventory = Cast<AEquipmentInventory>(Inventory.GetObject()))
            {
                AllItems.Append(EquipmentInventory->GetInventory());
            }
            else if (AConsumableInventory* ConsumableInventory = Cast<AConsumableInventory>(Inventory.GetObject()))
            {
                AllItems.Append(ConsumableInventory->GetInventory());
            }
        }

        // UI에 병합된 아이템 목록을 전달
        InventoryMainWidgetInstance->UpdateInventoryList(AllItems);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Item not added to any inventory."));
    }
}


void AInventoryManager::SetInventoryWidget(UInventoryMainWidget* NewWidget)
{
    InventoryMainWidgetInstance = NewWidget;
}

// Called when the game starts or when spawned
void AInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventoryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


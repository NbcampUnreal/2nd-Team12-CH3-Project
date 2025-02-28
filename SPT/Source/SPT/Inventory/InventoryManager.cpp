// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "SPT/Inventory/ItemWidget/InventoryMainWidget.h"

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

    if (Item->ItemActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem: ItemActorClass is valid for item %s"), *Item->ItemName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AddItem: ItemActorClass is nullptr for item %s"), *Item->ItemName);
    }

    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : AddItemToInventory : ItemName is %s"), *Item->ItemName);

    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory)
        {
            // 모든 인벤토리에서 AddItem 호출
            Inventory->AddItem(Item);
            UE_LOG(LogTemp, Warning, TEXT("Added to Inventory"));
            break;  // 첫 번째 인벤토리만 추가
        }
    }

    // 아이템 추가 직후 UI 갱신 보장
    if (InventoryMainWidgetInstance)
    {
        TArray<UInventoryItem*> AllItems;
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            if (Inventory)
            {
                TArray<UInventoryItem*> Items = Inventory->GetInventory();
                AllItems.Append(Items);
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("Inventory UI Update - Item Count: %d"), AllItems.Num());
        InventoryMainWidgetInstance->UpdateInventoryList(AllItems);
    }
}

void AInventoryManager::UseItem(UInventoryItem* Item)
{
    if (!Item) return;
    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : UseItem"));
    Item->UseItem();
}

void AInventoryManager::DropItem(UInventoryItem* Item, FVector DropLocation)
{
    UE_LOG(LogTemp, Warning, TEXT("InventoryManager : DropItem"));

    if (!Item->ItemActorClass)
    {
        UE_LOG(LogTemp, Error, TEXT("DropItem: Missing ItemActorClass for item %s"), *Item->ItemName);
        return;
    }

    if (!Item || !Item->ItemActorClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DropItem: Invalid item or missing ItemActorClass"));
        return;
    }

    // 아이템을 스폰
    AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(Item->ItemActorClass, DropLocation, FRotator::ZeroRotator);

    if (SpawnedItem)
    {

        // 아이템을 가시화하고 충돌 가능하게 설정
        SpawnedItem->SetActorHiddenInGame(false);
        SpawnedItem->SetActorEnableCollision(true);

        UE_LOG(LogTemp, Warning, TEXT("Item dropped at location: %s"), *DropLocation.ToString());

        // 인벤토리에서 아이템 제거
        RemoveItem(Item);
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn dropped item."));
    }
}

void AInventoryManager::RemoveItem(UInventoryItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Item is nullptr!"));
        return;
    }

    bool bItemRemoved = false;

    // 모든 인벤토리를 순회하면서 아이템 제거 시도
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory->RemoveItem(Item))  // 인터페이스를 통해 제거
        {
            UE_LOG(LogTemp, Warning, TEXT("Removed Item from Inventory"));
            bItemRemoved = true;
            break;  // 한 개의 인벤토리에서만 제거
        }
    }

    // 아이템이 제거되었으면 UI 갱신
    if (bItemRemoved && InventoryMainWidgetInstance)
    {
        TArray<UInventoryItem*> AllItems;
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AllItems.Append(Inventory->GetInventory());
        }
        InventoryMainWidgetInstance->UpdateInventoryList(AllItems);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveItem: Item not found in any inventory."));
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


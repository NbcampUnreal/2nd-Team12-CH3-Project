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
            // ��� �κ��丮���� AddItem ȣ��
            Inventory->AddItem(Item);
            UE_LOG(LogTemp, Warning, TEXT("Added to Inventory"));
            break;  // ù ��° �κ��丮�� �߰�
        }
    }

    // ������ �߰� ���� UI ���� ����
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

    // �������� ����
    AItemBase* SpawnedItem = GetWorld()->SpawnActor<AItemBase>(Item->ItemActorClass, DropLocation, FRotator::ZeroRotator);

    if (SpawnedItem)
    {

        // �������� ����ȭ�ϰ� �浹 �����ϰ� ����
        SpawnedItem->SetActorHiddenInGame(false);
        SpawnedItem->SetActorEnableCollision(true);

        UE_LOG(LogTemp, Warning, TEXT("Item dropped at location: %s"), *DropLocation.ToString());

        // �κ��丮���� ������ ����
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

    // ��� �κ��丮�� ��ȸ�ϸ鼭 ������ ���� �õ�
    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        if (Inventory->RemoveItem(Item))  // �������̽��� ���� ����
        {
            UE_LOG(LogTemp, Warning, TEXT("Removed Item from Inventory"));
            bItemRemoved = true;
            break;  // �� ���� �κ��丮������ ����
        }
    }

    // �������� ���ŵǾ����� UI ����
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


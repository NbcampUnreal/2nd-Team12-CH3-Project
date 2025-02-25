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

    // ��� ������ �߰�
    if (UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(Item))
    {
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AEquipmentInventory* EquipmentInventory = Cast<AEquipmentInventory>(Inventory.GetObject());
            if (EquipmentInventory)
            {
                EquipmentInventory->AddItem(EquipmentItem);  // ��� ������ �߰�
                UE_LOG(LogTemp, Warning, TEXT("Added to EquipmentInventory"));
                bItemAdded = true;
                break;  // ù ��° ��� �κ��丮�� �߰�
            }
        }
    }

    // �Ҹ�ǰ ������ �߰�
    if (!bItemAdded && Cast<UConsumableItem>(Item))
    {
        for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
        {
            AConsumableInventory* ConsumableInventory = Cast<AConsumableInventory>(Inventory.GetObject());
            if (ConsumableInventory)
            {
                ConsumableInventory->AddItem(Cast<UConsumableItem>(Item));  // �Ҹ�ǰ ������ �߰�
                UE_LOG(LogTemp, Warning, TEXT("Added to ConsumableInventory"));
                bItemAdded = true;
                break;  // ù ��° �Ҹ�ǰ �κ��丮�� �߰�
            }
        }
    }

    // �������� �߰��Ǿ����� UI ����
    if (bItemAdded && InventoryMainWidgetInstance)
    {
        // ��� �κ��丮���� ������ ����� ������ ����
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

        // UI�� ���յ� ������ ����� ����
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


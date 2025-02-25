// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"
#include "InventoryInterface.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"

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

void AInventoryManager::AddEquipmentItem(UEquipmentItem* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("AddEquipmentItem: Item is nullptr!"));
        return;
    }

    for (TScriptInterface<IInventoryInterface> Inventory : Inventories)
    {
        // IInventoryInterface�� ������ ���Ͱ� AEquipmentInventory���� Ȯ��
        AEquipmentInventory* EquipmentInventory = Cast<AEquipmentInventory>(Inventory.GetObject());
        if (EquipmentInventory)
        {
            EquipmentInventory->AddItem(Item);
            UE_LOG(LogTemp, Warning, TEXT("Added to EquipmentInventory"));
            return; // ù ��° �߰ߵ� ��� �κ��丮���� �߰�
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("No EquipmentInventory found to add"));
}

void AInventoryManager::AddConsumableItem(UConsumableItem* Item)
{

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


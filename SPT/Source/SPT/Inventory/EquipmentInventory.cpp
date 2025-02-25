// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentInventory.h"

AEquipmentInventory::AEquipmentInventory()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEquipmentInventory::AddItem(UInventoryItem* Item)
{
	if (UEquipmentItem* Equipment = Cast<UEquipmentItem>(Item))
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipment Inventory AddItem!"));
		EquipmentItems.Add(Equipment);
	}
}

void AEquipmentInventory::DropItem(UInventoryItem* Item)
{
	EquipmentItems.Remove(Cast<UEquipmentItem>(Item));
}

bool AEquipmentInventory::HasItem(FName ItemName)
{
	return EquipmentItems.ContainsByPredicate([ItemName](UEquipmentItem* Item)
	{
		return Item->ItemName == ItemName;
	});
}

const TArray<UInventoryItem*>& AEquipmentInventory::GetInventory() const
{
	return reinterpret_cast<const TArray<UInventoryItem*>&>(EquipmentItems);
}

void AEquipmentInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEquipmentInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


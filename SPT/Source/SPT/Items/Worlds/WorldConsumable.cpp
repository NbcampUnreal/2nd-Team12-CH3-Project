// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldConsumable.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Consumables/ConsumableItem.h"
#include "Engine/World.h"

AWorldConsumable::AWorldConsumable()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWorldConsumable::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldConsumable::OnPickup - PlayerCharacter is null!"));
		return;
	}
	
	// �Һ� ������ ������ ��ü ����
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	if (NewItemData)
	{
		// �÷��̾� �κ��丮�� �߰� (���� �ʿ�)
		// PlayerCharacter->AddToInventory(NewItemData);
		UE_LOG(LogTemp, Log, TEXT("Consumable picked up and added to inventory: %s"), *NewItemData->GetItemData().TextData.ItemName.ToString());
	}

	// ���忡�� ����
	Destroy();
}

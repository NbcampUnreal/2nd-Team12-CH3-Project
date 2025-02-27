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
	
	// 소비 아이템 데이터 객체 생성
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	if (NewItemData)
	{
		// 플레이어 인벤토리에 추가 (구현 필요)
		// PlayerCharacter->AddToInventory(NewItemData);
		UE_LOG(LogTemp, Log, TEXT("Consumable picked up and added to inventory: %s"), *NewItemData->GetItemData().TextData.ItemName.ToString());
	}

	// 월드에서 제거
	Destroy();
}

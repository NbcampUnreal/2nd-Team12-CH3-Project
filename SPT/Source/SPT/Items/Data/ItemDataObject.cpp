// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"

UItemDataObject::UItemDataObject()
{
	Quantity = 1;
}

UItemDataObject* UItemDataObject::CreateItemCopy() const
{
	UItemDataObject* NewItem = NewObject<UItemDataObject>();
	if (NewItem)
	{
		NewItem->SetItemData(ItemData);
		NewItem->SetQuantity(Quantity);
	}
	return NewItem;
}

float UItemDataObject::GetItemStackWeight() const
{
	return ItemData.NumericData.Weight * Quantity;
}

float UItemDataObject::GetItemSingleWeight() const
{
	return ItemData.NumericData.Weight;
}

bool UItemDataObject::IsFullItemStack() const
{
	return Quantity >= ItemData.NumericData.MaxStackSize;
}

void UItemDataObject::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemObject: Cannot Use - Character is null!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using item: %s"), *ItemData.TextData.Name.ToString());

	// TODO: 아이템 사용 효과 적용

	// 소모품이면 사용 후 개수 줄이기
	if (IsConsumable())
	{
		SetQuantity(Quantity - 1);
		if (Quantity <= 0)
		{
			// TODO: 인벤토리에서 제거
		}
	}
}

FItemData UItemDataObject::GetItemData() const
{
	return ItemData;
}

void UItemDataObject::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
	Quantity = 1;
}

int32 UItemDataObject::GetQuantity() const
{
	return Quantity;
}

void UItemDataObject::SetQuantity(const int32 NewQuantity)
{
	Quantity = FMath::Clamp(
		NewQuantity,
		0,
		ItemData.NumericData.bIsStackable ? ItemData.NumericData.MaxStackSize : 1);
}

bool UItemDataObject::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
}

bool UItemDataObject::IsConsumable() const
{
	return ItemData.ItemType == EItemType::EIT_Consumable;
}
/*
bool UItemDataObject::IsInInventory() const
{
	return OwningInventory != nullptr;
}
*/

bool UItemDataObject::operator==(const FName& OtherID) const
{
	return  this->ItemData.ItemID == OtherID;
}

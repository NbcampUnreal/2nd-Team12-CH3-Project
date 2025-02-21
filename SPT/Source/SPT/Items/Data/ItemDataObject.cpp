// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"
#include "SPT/Characters/SPTPlayerCharacter.h"

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

	// 소비 아이템 효과 적용 (예: 체력 회복, 버프)
	if (IsConsumable())
	{
		ApplyConsumableEffect(PlayerCharacter);

		// 사용 후 개수 감소
		SetQuantity(Quantity - 1);

		// 개수가 0이 되면 인벤토리에서 제거
		if (Quantity <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("%s has been consumed completely! Removing from inventory."), *ItemData.TextData.Name.ToString());
			/*
			if (PlayerCharacter->InventoryComponent)
			{
				PlayerCharacter->InventoryComponent->RemoveItem(this);
			}
			else
			{
				ConditionalBeginDestroy(); // 안전하게 객체 제거
			}
			*/
		}
	}
}

void UItemDataObject::ApplyConsumableEffect(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Applying effect of consumable item: %s"), *ItemData.TextData.Name.ToString());

	// TODO: 체력 회복, 스태미너 회복, 버프 등 적용 로직 추가
	// 예시: 체력 회복
	// PlayerCharacter->IncreaseHealth(ItemData.NumericData.Value);
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

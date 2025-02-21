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

	// �Һ� ������ ȿ�� ���� (��: ü�� ȸ��, ����)
	if (IsConsumable())
	{
		ApplyConsumableEffect(PlayerCharacter);

		// ��� �� ���� ����
		SetQuantity(Quantity - 1);

		// ������ 0�� �Ǹ� �κ��丮���� ����
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
				ConditionalBeginDestroy(); // �����ϰ� ��ü ����
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

	// TODO: ü�� ȸ��, ���¹̳� ȸ��, ���� �� ���� ���� �߰�
	// ����: ü�� ȸ��
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

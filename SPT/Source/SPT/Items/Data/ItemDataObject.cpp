// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/ConsumableItems/ConsumableItemActor.h"
#include "Engine/World.h"

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

void UItemDataObject::Use(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemObject: Cannot Use - Character is null!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Using item: %s"), *ItemData.TextData.Name.ToString());

	// �Һ� �������� ��� ���� ���� ��ȯ
	if (IsConsumable())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;
		SpawnParams.Instigator = PlayerCharacter;

		AConsumableItemActor* SpawnedItem = PlayerCharacter->GetWorld()->SpawnActor<AConsumableItemActor>(
			AConsumableItemActor::StaticClass(),
			PlayerCharacter->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParams
		);

		if (SpawnedItem)
		{
			SpawnedItem->SetItemData(ItemData);
			SpawnedItem->Use(PlayerCharacter);
		}
	}

	// ������ ���� ���� ��, 0�� �Ǹ� �κ��丮���� ����
	SetQuantity(Quantity - 1);
	if (Quantity <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("%s has been consumed completely! Removing from inventory."), *ItemData.TextData.Name.ToString());
		// TODO: �κ��丮���� ������ ���� ó�� �ʿ�
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

bool UItemDataObject::CanMergeWith(const UItemDataObject* OtherItem) const
{
	if (!OtherItem)
	{
		return false;
	}
	// �� �������� ���� ����, ���� ������ ID�� ��� ���� ����
	return ItemData.NumericData.bIsStackable && (ItemData.ItemID == OtherItem->GetItemData().ItemID);
}
int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!OtherItem || !CanMergeWith(OtherItem))
	{
		// ���� �Ұ����� ��� OtherItem�� ���� ���� �״�� ��ȯ
		return OtherItem ? OtherItem->GetQuantity() : 0;
	}

	// �ִ� ���� ũ��� ���� ������ ���� ���� ���� ���
	int32 MaxStack = ItemData.NumericData.MaxStackSize;
	int32 AvailableSpace = MaxStack - Quantity;
	int32 OtherQuantity = OtherItem->GetQuantity();
	int32 MergedAmount = FMath::Min(AvailableSpace, OtherQuantity);

	// ���� ���ÿ� ����
	SetQuantity(Quantity + MergedAmount);
	// OtherItem�� ���� ����
	OtherItem->SetQuantity(OtherQuantity - MergedAmount);

	// ���� �� OtherItem�� ���� ���� ��ȯ
	return OtherItem->GetQuantity();
}
UItemDataObject* UItemDataObject::SplitStack(int32 SplitQuantity)
{
	if (SplitQuantity <= 0 || SplitQuantity >= Quantity)
	{
		return nullptr;
	}

	// ���� ���ÿ��� �и��� ��ŭ ����
	SetQuantity(Quantity - SplitQuantity);

	// ���ο� UItemDataObject ���� (Outer�� ���� ��ü�� Outer ���)
	UItemDataObject* NewStack = NewObject<UItemDataObject>(this->GetOuter(), this->GetClass());
	if (NewStack)
	{
		NewStack->SetItemData(ItemData);
		NewStack->SetQuantity(SplitQuantity);
	}
	return NewStack;
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

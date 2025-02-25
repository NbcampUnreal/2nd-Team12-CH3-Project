// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataObject.h"

UItemDataObject::UItemDataObject()
{
	Quantity = 1;
	ItemDataTable = nullptr;
	WeaponDataTable = nullptr;
}

void UItemDataObject::InitializeFromDataTable(FName RowName)
{
	if (!ItemDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid DataTable"));
		return;
	}

	// FItemData ����ü���� ������ �ε�
	FItemData* FoundItemData = ItemDataTable->FindRow<FItemData>(RowName, TEXT(""));
	if (FoundItemData)
	{
		ItemData = *FoundItemData;  // ������ �����͸� �ε�
		Quantity = 1; // �⺻ �� ����
	}

	// FWeaponData ����ü���� ������ �ε�
	if (IsWeapon() && WeaponDataTable)
	{
		FWeaponItemData* FoundWeaponData = WeaponDataTable->FindRow<FWeaponItemData>(RowName, TEXT(""));
		if (FoundWeaponData)
		{
			WeaponData = *FoundWeaponData;
		}
	}
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

FItemData UItemDataObject::GetItemData() const
{
	return ItemData;
}

void UItemDataObject::SetItemData(const FItemData& NewItemData)
{
	ItemData = NewItemData;
}

const FWeaponItemData& UItemDataObject::GetWeaponData() const
{
	return WeaponData;
}

void UItemDataObject::SetWeaponData(const FWeaponItemData& NewWeaponData)
{
	WeaponData = NewWeaponData;
}

bool UItemDataObject::IsWeapon() const
{
	return ItemData.ItemType == EItemType::EIT_Weapon;
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
		ItemData.NumericData.MaxStackSize);
}

bool UItemDataObject::CanStackWith(const UItemDataObject* OtherItem) const
{
	// ���� ���� ���� Ȯ�� (�������� ���� Ÿ���̰�, ���� ���� ����)
	return (ItemData.ItemID == OtherItem->ItemData.ItemID) && ItemData.NumericData.bIsStackable;
}

int32 UItemDataObject::MergeStack(UItemDataObject* OtherItem)
{
	if (!OtherItem || !CanStackWith(OtherItem)) return 0;

	// ���� ������ �ִ� ������ŭ�� ����
	int32 MergedAmount = FMath::Min(ItemData.NumericData.MaxStackSize - Quantity, OtherItem->Quantity);
	Quantity += MergedAmount;
	OtherItem->SetQuantity(OtherItem->Quantity - MergedAmount);

	return MergedAmount;
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

bool UItemDataObject::operator==(const FName& OtherID) const
{
	return  this->ItemData.ItemID == OtherID;
}

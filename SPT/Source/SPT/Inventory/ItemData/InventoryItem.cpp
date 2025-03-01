// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Base/ItemBase.h"



// �̱���
void UInventoryItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemData : UseItem"));
}

// �������� �����͸� ������ ����
void UInventoryItem::SetItemData(UItemDataObject* NewItemDataObject)
{
    if (NewItemDataObject) // ������ �����Ͱ� ��ȿ���� Ȯ��
    {
        // CreateItemCopy()�� ����Ͽ� ���ο� ���纻 ����
        ItemDataObject = NewItemDataObject->CreateItemCopy();
    }
}

// �������� Ŭ������ ȣ���ϱ� ���� ���� �Լ�
// ������ �ʿ� ���δ� Ȯ�� �ʿ�(�ʿ����� �������� ����)
TSubclassOf<AItemBase> UInventoryItem::GetItemBaseClass() const
{
    if (ItemDataObject)
    {
        if (ItemDataObject->ItemData.ItemBaseClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("GetItemBaseClass: ItemBaseClass is valid"));
            return ItemDataObject->ItemData.ItemBaseClass;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GetItemBaseClass: ItemBaseClass is nullptr"));
            UE_LOG(LogTemp, Error, TEXT("GetItemBaseClass: ItemName is %s"), *GetItemName().ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetItemBaseClass: ItemDataObject is nullptr"));
    }

    return nullptr;
}

FText UInventoryItem::GetItemName() const
{
	return ItemDataObject ? ItemDataObject->ItemData.TextData.ItemName : FText::FromString(TEXT("Unknown"));
}

UTexture2D* UInventoryItem::GetItemIcon() const
{
    return ItemDataObject ? ItemDataObject->Icon : nullptr;
}

// �������� Ÿ�� �Ǻ�
bool UInventoryItem::IsWeapon() const
{
    return ItemDataObject && ItemDataObject->IsWeapon();
}

bool UInventoryItem::IsConsumable() const
{
    return ItemDataObject && ItemDataObject->IsConsumable();
}

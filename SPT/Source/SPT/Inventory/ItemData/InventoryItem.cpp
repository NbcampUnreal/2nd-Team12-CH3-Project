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
        if (ItemDataObject->GetItemData().ItemBaseClass)
        {
            return ItemDataObject->GetItemData().ItemBaseClass;
        }
    }
    return nullptr;
}

FText UInventoryItem::GetItemName() const
{
	return ItemDataObject ? ItemDataObject->GetItemData().TextData.ItemName : FText::FromString(TEXT("Unknown"));
}

UTexture2D* UInventoryItem::GetItemIcon() const
{
    return ItemDataObject ? ItemDataObject->GetItemData().AssetData.Icon : nullptr;
}

USkeletalMesh* UInventoryItem::GetSkeletalMesh() const
{
    return ItemDataObject ? ItemDataObject->GetItemData().AssetData.SkeletalMesh : nullptr;
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

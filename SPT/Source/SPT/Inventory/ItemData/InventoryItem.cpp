// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "SPT/Items/Data/ItemDataObject.h"
#include "SPT/Items/Base/ItemBase.h"
#include "SPT/Items/Weapons/WeaponBase.h"



// 미구현
void UInventoryItem::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemData : UseItem"));
}

// 아이템의 데이터를 가져와 복제
void UInventoryItem::SetItemData(UItemDataObject* NewItemDataObject)
{
    if (NewItemDataObject) // 아이템 데이터가 유효한지 확인
    {
        // CreateItemCopy()를 사용하여 새로운 복사본 생성
        ItemDataObject = NewItemDataObject->CreateItemCopy();
    }
}

// 아이템의 클래스를 호출하기 위해 만든 함수
// 하지만 필요 여부는 확인 필요(필요하지 않을수도 있음)
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

TSubclassOf<AWeaponBase> UInventoryItem::GetWeaponClass() const
{
    if (ItemDataObject)
    {
        if (ItemDataObject->WeaponData.WeaponClass)
        {
            return TSubclassOf<AWeaponBase>(ItemDataObject->WeaponData.WeaponClass);
        }
    }
    return nullptr;
}

void UInventoryItem::SetItemBase(AItemBase* NewItemBase)
{
    ItemBaseReference = NewItemBase;
}

AItemBase* UInventoryItem::GetItemBase() const
{
    return ItemBaseReference;
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

// 아이템의 타입 판별
bool UInventoryItem::IsWeapon() const
{
    return ItemDataObject && ItemDataObject->IsWeapon();
}

bool UInventoryItem::IsConsumable() const
{
    return ItemDataObject && ItemDataObject->IsConsumable();
}

int32 UInventoryItem::GetSlotType() const
{
    // 임시로 무기는 0으로 하였습니다.
    if (ItemDataObject->IsWeapon()) 
    {
        return 0;
    }
    // 임시로 소모품은 5로 하였습니다. 그냥 -1로 해도 상관없을지도?
    else if (ItemDataObject->IsConsumable()) 
    {
        return 5;
    }
    return -1;
}

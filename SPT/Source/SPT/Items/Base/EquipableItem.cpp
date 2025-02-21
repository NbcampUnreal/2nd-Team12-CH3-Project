// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"

AEquipableItem::AEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;
    DetermineAttachSocket();
}

void AEquipableItem::DetermineAttachSocket()
{
    // 아이템 데이터가 유효한 경우 소켓 이름을 설정
    if (!ItemData.AssetData.AttachSocketName.IsNone())
    {
        AttachSocketName = ItemData.AssetData.AttachSocketName;
    }
    else
    {
        // 기본값 설정 (무기: 손, 나머지: 허리)
        AttachSocketName = (ItemData.ItemType == EItemType::EIT_Weapon) ? "hand_r" : "spine";
    }
}

bool AEquipableItem::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        return false;
    }

    if (PlayerCharacter->EquippedItem)
    {
        PlayerCharacter->UnEquipItem(); // 기존 장착 아이템 해제
    }

    PlayerCharacter->EquippedItem = this;
    AttachToComponent(
        PlayerCharacter->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocketName);

    UpdateItemState(EItemState::EIS_Equipped);
    return true;
}

bool AEquipableItem::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->EquippedItem)
    {
        return false;
    }

    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    PlayerCharacter->EquippedItem = nullptr;
    UpdateItemState(EItemState::EIS_QuickSlot);

    return true;
}

void AEquipableItem::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || !PlayerCharacter->EquippedItem)
    {
        return;
    }

    AWorldItemActor* DroppedItem = PlayerCharacter->GetWorld()->SpawnActor<AWorldItemActor>(
        AWorldItemActor::StaticClass(),
        PlayerCharacter->GetActorLocation() + FVector(50, 0, 0),
        FRotator::ZeroRotator);

    if (DroppedItem)
    {
        DroppedItem->InitializeItem(ItemData);
    }

    PlayerCharacter->UnEquipItem();
    Destroy();
}

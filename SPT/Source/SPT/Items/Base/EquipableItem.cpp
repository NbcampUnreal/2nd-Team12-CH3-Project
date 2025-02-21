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
    // ������ �����Ͱ� ��ȿ�� ��� ���� �̸��� ����
    if (!ItemData.AssetData.AttachSocketName.IsNone())
    {
        AttachSocketName = ItemData.AssetData.AttachSocketName;
    }
    else
    {
        // �⺻�� ���� (����: ��, ������: �㸮)
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
        PlayerCharacter->UnEquipItem(); // ���� ���� ������ ����
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

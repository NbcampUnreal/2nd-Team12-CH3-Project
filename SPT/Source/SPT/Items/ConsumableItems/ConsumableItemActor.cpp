// Fill out your copyright notice in the Description page of Project Settings.


#include "ConsumableItemActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
#include "SPT/Items/WorldItems/WorldItemActor.h"

AConsumableItemActor::AConsumableItemActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AConsumableItemActor::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        return false;
    }

    // 소비 아이템은 손에 들지 않고 퀵슬롯에 저장됨
    PlayerCharacter->ConsumableQuickSlot = this;
    UpdateItemState(EItemState::EIS_QuickSlot);
    return true;
}

bool AConsumableItemActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || PlayerCharacter->ConsumableQuickSlot != this)
    {
        return false;
    }

    PlayerCharacter->ConsumableQuickSlot = nullptr;
    UpdateItemState(EItemState::EIS_Inventory);
    return true;
}

void AConsumableItemActor::Use(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Using consumable: %s"), *GetName());

    // TODO: 체력 회복 또는 버프 적용 등의 효과 구현

    Destroy();
}

void AConsumableItemActor::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter || PlayerCharacter->ConsumableQuickSlot != this)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = nullptr;
    SpawnParams.Instigator = PlayerCharacter;

    AWorldItemActor* DroppedItem = PlayerCharacter->GetWorld()->SpawnActor<AWorldItemActor>(
        AWorldItemActor::StaticClass(),
        PlayerCharacter->GetActorLocation() + FVector(50, 0, 0),
        FRotator::ZeroRotator,
        SpawnParams);

    if (DroppedItem)
    {
        DroppedItem->InitializeItem(ItemData);
    }

    PlayerCharacter->ConsumableQuickSlot = nullptr;
    Destroy();
}


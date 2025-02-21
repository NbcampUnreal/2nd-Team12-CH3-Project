// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"
#include "SPT/Characters/SPTPlayerCharacter.h"

AEquipableItem::AEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AEquipableItem::CanEquip(ASPTPlayerCharacter* PlayerCharacter) const
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: PlayerCharacter is null!"));
        return false;
    }

    // 예시: 무기가 이미 장착되어 있지 않은 경우에만 장착 가능
    return true;
}

bool AEquipableItem::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Cannot Equip - Character is null!"));
        return false;
    }

    if (!CanEquip(PlayerCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Cannot Equip - Conditions not met"));
        return false;
    }

    // 아이템을 캐릭터에 장착 (예: 무기 슬롯에 추가)
    // 캐릭터의 Mesh에 부착
    SkeletalMeshComponent->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
    UE_LOG(LogTemp, Log, TEXT("%s Equipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: 캐릭터의 장비 슬롯에 추가하는 로직

    return true;
}

bool AEquipableItem::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Cannot Unequip - Character is null!"));
        return false;
    }

    // 장착 해제 로직
    SkeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    UE_LOG(LogTemp, Log, TEXT("%s Unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: 캐릭터의 인벤토리로 아이템 반환

    return true;
}
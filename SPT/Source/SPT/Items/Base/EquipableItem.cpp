// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipableItem.h"
#include "SPT/Characters/SPTPlayerCharacter.h"

AEquipableItem::AEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;
    DetermineAttachSocket();
}

void AEquipableItem::DetermineAttachSocket()
{
    // 무기인지 확인
    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        AttachSocketName = "hand_r"; // 무기는 손에 장착
    }
    else
    {
        AttachSocketName = "spine"; // 기타 아이템은 허리에 장착
    }
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
        return false;
    }

    if (!CanEquip(PlayerCharacter))
    {
        return false;
    }

    USkeletalMeshComponent* CharacterMesh = PlayerCharacter->GetMesh();
    if (!CharacterMesh)
    {
        return false;
    }

    if (AttachSocketName.IsNone() || !CharacterMesh->DoesSocketExist(AttachSocketName))
    {
        return false;
    }

    SkeletalMeshComponent->AttachToComponent(
        CharacterMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocketName);

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
    SetOwner(nullptr);
    UE_LOG(LogTemp, Log, TEXT("%s Unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: 캐릭터의 인벤토리로 아이템 반환

    return true;
}
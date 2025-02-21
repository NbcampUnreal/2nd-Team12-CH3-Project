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
    // �������� Ȯ��
    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        AttachSocketName = "hand_r"; // ����� �տ� ����
    }
    else
    {
        AttachSocketName = "spine"; // ��Ÿ �������� �㸮�� ����
    }
}

bool AEquipableItem::CanEquip(ASPTPlayerCharacter* PlayerCharacter) const
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: PlayerCharacter is null!"));
        return false;
    }

    // ����: ���Ⱑ �̹� �����Ǿ� ���� ���� ��쿡�� ���� ����
    return true;
}

bool AEquipableItem::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
    UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Equip() called for %s"), *GetName()); // �α� ���� Warning���� ����

    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipableItem: Cannot Equip - Character is null!"));
        return false;
    }

    if (!CanEquip(PlayerCharacter))
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Cannot Equip - Conditions not met for %s"), *GetName());
        return false;
    }

    USkeletalMeshComponent* CharacterMesh = PlayerCharacter->GetMesh();
    if (!CharacterMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("EquipableItem: Failed to equip %s - PlayerCharacter's Mesh is null!"), *GetName());
        return false;
    }

    if (AttachSocketName.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("EquipableItem: Failed to equip %s - AttachSocketName is not set!"), *GetName());
        return false;
    }

    if (!CharacterMesh->DoesSocketExist(AttachSocketName))
    {
        UE_LOG(LogTemp, Error, TEXT("EquipableItem: Failed to equip %s - AttachSocketName '%s' does not exist on PlayerCharacter's Mesh!"),
            *GetName(), *AttachSocketName.ToString());
        return false;
    }

    SkeletalMeshComponent->AttachToComponent(
        CharacterMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocketName);

    UE_LOG(LogTemp, Warning, TEXT("1111 - %s Equipped by %s"), *GetName(), *PlayerCharacter->GetName()); // �α� ���� Warning���� ����

    return true;

}

bool AEquipableItem::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipableItem: Cannot Unequip - Character is null!"));
        return false;
    }

    // ���� ���� ����
    SkeletalMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    SetOwner(nullptr);
    UE_LOG(LogTemp, Log, TEXT("%s Unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: ĳ������ �κ��丮�� ������ ��ȯ

    return true;
}
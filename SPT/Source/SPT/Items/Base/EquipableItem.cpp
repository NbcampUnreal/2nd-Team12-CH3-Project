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

    // ����: ���Ⱑ �̹� �����Ǿ� ���� ���� ��쿡�� ���� ����
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

    // �������� ĳ���Ϳ� ���� (��: ���� ���Կ� �߰�)
    // ĳ������ Mesh�� ����
    SkeletalMeshComponent->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
    UE_LOG(LogTemp, Log, TEXT("%s Equipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: ĳ������ ��� ���Կ� �߰��ϴ� ����

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
    UE_LOG(LogTemp, Log, TEXT("%s Unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

    // TODO: ĳ������ �κ��丮�� ������ ��ȯ

    return true;
}
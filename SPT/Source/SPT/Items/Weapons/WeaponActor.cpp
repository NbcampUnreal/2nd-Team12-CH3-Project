// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	AttachSocketName = "hand_r";
}

bool AWeaponActor::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return false;
	}

	WeaponState = EItemState::EIS_Equipped;

	// ĳ������ SkeletalMesh���� �� ������ ������ ���⸦ ����
	USkeletalMeshComponent* CharacterMesh = PlayerCharacter->GetMesh();
	if (!CharacterMesh)
	{
		return false;
	}

	// ���⸦ �� ����("hand_r")�� ����
	AttachToComponent(
		CharacterMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		AttachSocketName
	);

	// ���� ���� �ִϸ��̼� ���� (���� ���)
	/*
	if (ItemData.EquipAnimation && PlayerCharacter->GetMesh()->GetAnimInstance())
	{
		PlayerCharacter->PlayAnimMontage(ItemData.EquipAnimation);
	}
	*/

	UE_LOG(LogTemp, Log, TEXT("AWeaponActor: %s is Equipped (Socket: hand_r)"), *GetName());

	return true;
}

bool AWeaponActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s is UnEquipped!"), *GetName());

	WeaponState = EItemState::EIS_Inventory;

	// TODO: ĳ������ ���� ���Կ��� �����ϴ� ����

	return true;
}

void AWeaponActor::Drop()
{
	UE_LOG(LogTemp, Log, TEXT("%s ���� ���!"), *GetName());

	WeaponState = EItemState::EIS_World;

	// TODO: ���忡 ���� ��� ���� �߰�
}

FWeaponItemData AWeaponActor::GetWeaponData() const
{
	return WeaponData;
}

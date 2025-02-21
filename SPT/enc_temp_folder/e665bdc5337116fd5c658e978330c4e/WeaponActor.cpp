// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AWeaponActor::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("%s ���� ������!"), *GetName());

	WeaponState = EItemState::EIS_Equipped;

	// TODO: ĳ������ ���� ���Կ� �߰��ϴ� ����

	return true;
}

bool AWeaponActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s ���� ������!"), *GetName());

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

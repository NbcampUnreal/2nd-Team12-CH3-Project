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

	// WeaponState = EItemState::Equipped;

	// TODO: ĳ������ ���� ���Կ� �߰��ϴ� ����

	return true;
}

bool AWeaponActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s ���� ������!"), *GetName());

	// WeaponState = EItemState::Unequipped;

	// TODO: ĳ������ ���� ���Կ��� �����ϴ� ����

	return true;
}

void AWeaponActor::Drop()
{
	UE_LOG(LogTemp, Log, TEXT("%s ���� ���!"), *GetName());

	// WeaponState = EItemState::Dropped;

	// TODO: ���忡 ���� ��� ���� �߰�
}

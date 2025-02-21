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

	UE_LOG(LogTemp, Log, TEXT("%s 무기 장착됨!"), *GetName());

	// WeaponState = EItemState::Equipped;

	// TODO: 캐릭터의 무기 슬롯에 추가하는 로직

	return true;
}

bool AWeaponActor::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("%s 무기 해제됨!"), *GetName());

	// WeaponState = EItemState::Unequipped;

	// TODO: 캐릭터의 무기 슬롯에서 제거하는 로직

	return true;
}

void AWeaponActor::Drop()
{
	UE_LOG(LogTemp, Log, TEXT("%s 무기 드롭!"), *GetName());

	// WeaponState = EItemState::Dropped;

	// TODO: 월드에 무기 드롭 로직 추가
}

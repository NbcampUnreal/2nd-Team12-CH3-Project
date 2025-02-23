// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "SPT/Characters/SPTPlayerCharacter.h"
// #include "SPT/Inventory/InventoryComponent.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponActor::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	/*
	if (!PlayerCharacter || !PlayerCharacter->InventoryComponent)
	{
		return;
	}
	*/

	// 무기 장착 (기존 장착 무기 해제 후)
	if (PlayerCharacter->EquippedItem)
	{
		PlayerCharacter->UnEquipItem();
	}

	// 플레이어의 손에 무기를 장착
	PlayerCharacter->EquipItem(this);

	UE_LOG(LogTemp, Log, TEXT("WeaponActor: %s picked up %s"), *PlayerCharacter->GetName(), *GetName());

	// 월드에서 제거
	Destroy();
}

void AWeaponActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// 아이템을 버릴 때 월드에 다시 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerCharacter;
	SpawnParams.Instigator = PlayerCharacter;

	AWeaponActor* DroppedWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		GetClass(),
		PlayerCharacter->GetActorLocation() + FVector(50, 0, 0),
		FRotator::ZeroRotator,
		SpawnParams);

	if (DroppedWeapon)
	{
		DroppedWeapon->SetItemData(GetItemData());
		UE_LOG(LogTemp, Log, TEXT("%s has dropped %s"), *PlayerCharacter->GetName(), *DroppedWeapon->GetName());
	}
}

EWeaponType AWeaponActor::GetWeaponType() const
{
	return GetWeaponData().WeaponType;
}

FWeaponItemData AWeaponActor::GetWeaponData() const
{
	return ItemData.WeaponData;
}

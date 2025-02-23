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

	// ���� ���� (���� ���� ���� ���� ��)
	if (PlayerCharacter->EquippedItem)
	{
		PlayerCharacter->UnEquipItem();
	}

	// �÷��̾��� �տ� ���⸦ ����
	PlayerCharacter->EquipItem(this);

	UE_LOG(LogTemp, Log, TEXT("WeaponActor: %s picked up %s"), *PlayerCharacter->GetName(), *GetName());

	// ���忡�� ����
	Destroy();
}

void AWeaponActor::OnDrop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	// �������� ���� �� ���忡 �ٽ� ����
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

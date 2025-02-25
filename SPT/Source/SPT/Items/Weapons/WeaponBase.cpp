// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "SPTPlayerCharacter.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetSimulatePhysics(false); // 물리 비활성화
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	}
}

void AWeaponBase::Equip()
{
}

void AWeaponBase::UnEquip()
{
}

void AWeaponBase::Drop()
{
}

void AWeaponBase::SwitchWeapon(AWeaponBase* NewWeapon)
{
}

const FWeaponItemData& AWeaponBase::GetWeaponData() const
{
	if (ItemData)
	{
		static FWeaponItemData DefaultWeaponData;
		UE_LOG(LogTemp, Warning, TEXT("AWeaponBase::GetWeaponData() - ItemData is null, returning default data"));
		return DefaultWeaponData;
	}
	return ItemData->GetWeaponData();
}

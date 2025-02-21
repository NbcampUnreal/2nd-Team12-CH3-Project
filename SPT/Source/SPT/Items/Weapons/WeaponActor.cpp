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

	// 캐릭터의 SkeletalMesh에서 손 소켓을 가져와 무기를 부착
	USkeletalMeshComponent* CharacterMesh = PlayerCharacter->GetMesh();
	if (!CharacterMesh)
	{
		return false;
	}

	// 무기를 손 소켓("hand_r")에 부착
	AttachToComponent(
		CharacterMesh,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		AttachSocketName
	);

	// 무기 장착 애니메이션 실행 (있을 경우)
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

	// TODO: 캐릭터의 무기 슬롯에서 제거하는 로직

	return true;
}

void AWeaponActor::Drop()
{
	UE_LOG(LogTemp, Log, TEXT("%s 무기 드롭!"), *GetName());

	WeaponState = EItemState::EIS_World;

	// TODO: 월드에 무기 드롭 로직 추가
}

FWeaponItemData AWeaponActor::GetWeaponData() const
{
	return WeaponData;
}

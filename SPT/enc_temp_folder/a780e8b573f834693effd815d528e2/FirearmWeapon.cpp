// Fill out your copyright notice in the Description page of Project Settings.


#include "FirearmWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Worlds/WorldWeapon.h"
#include "Engine/World.h"

AFirearmWeapon::AFirearmWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// Skeletal Mesh �߰�
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	if (StaticMeshComponent && SkeletalMeshComponent)
	{
		SkeletalMeshComponent->SetupAttachment(RootComponent);
	}

	// �ʱ� ���� ����
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ASPTPlayerCharacter>(GetOwner());
	/*
	if (FirearmStats.HolsterSocketName.IsValid())
	{
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirearmStats.HolsterSocketName);
	}
	*/
}

bool AFirearmWeapon::CanFire()
{
	if (bIsEquipping || bIsFiring || bIsReloading)
	{
		return false;
	}

	return true;;
}

void AFirearmWeapon::Begin_Fire()
{
	bIsFiring = true;
	Attack();
}

void AFirearmWeapon::End_Fire()
{
	if (!bIsFiring)
	{
		return;
	}

	bIsFiring = false;
}

void AFirearmWeapon::Attack()
{
	// TODO : �߻� ����, �κ��丮�� ����
	if (FirearmStats.AmmoCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo!"), *GetName());
		return;
	}

	// ��� ���·� ��ȯ
	FirearmStats.AmmoCount--;
	CurrentRecoil += FirearmStats.Recoil;  // �ݵ� ����

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
}

bool AFirearmWeapon::CanEquip()
{
	if (ItemState == EItemState::EIS_Equipped || bIsReloading || !bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	bIsEquipping = true;
	Owner = PlayerCharacter;
	/*
	if (FirearmStats.EquipMontage)
	{
		Owner->PlayAnimMontage(FirearmStats.EquipMontage, FirearmStats.EquipMontage_PlayRate);
	}
	*/

	Begin_Equip();
}

void AFirearmWeapon::Begin_Equip()
{
	// ������ ������ �ѱⰡ �ִٸ� ���� ����
	if (AWeaponBase* EquippedWeapon = Owner->GetEquippedWeapon())
	{
		if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
		{
			Drop(Owner);
		}
	}

	// ������ ��ġ�� �̵�
	// PlayerCharacter->EquipWeapon(this); ���
	if (FirearmStats.RightHandSocketName.IsValid())
	{
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirearmStats.RightHandSocketName);
	}

	ItemState = EItemState::EIS_Equipped;
	UE_LOG(LogTemp, Log, TEXT("%s equipped by %s"), *GetName(), *Owner->GetName());

	End_Equip();
}

void AFirearmWeapon::End_Equip()
{
	bIsEquipping = false;
}


bool AFirearmWeapon::CanUnEquip()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::UnEquip - PlayerCharacter is null!"));
		return;
	}

	if (FirearmStats.HolsterSocketName.IsValid())
	{
		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FirearmStats.HolsterSocketName);
		UE_LOG(LogTemp, Log, TEXT("%s unequipped by %s"), *GetName(), *PlayerCharacter->GetName());
	}
}

void AFirearmWeapon::Drop(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Drop - PlayerCharacter is null!"));
		return;
	}

	// ��� ��ġ ���
	FVector DropLocation;
	FRotator DropRotation;
	GetDropLocation(PlayerCharacter, DropLocation, DropRotation);

	// AWorldWeapon ����
	AWorldWeapon* DroppedWeapon = GetWorld()->SpawnActor<AWorldWeapon>(AWorldWeapon::StaticClass(), DropLocation, DropRotation);
	if (!DroppedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn AWorldWeapon!"));
		return;
	}

	// ���� ���� �����͸� AWorldWeapon�� ����
	ItemState = EItemState::EIS_World;
	WeaponData.FirearmStats = FirearmStats;
	ItemData->SetWeaponData(WeaponData);

	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	DroppedWeapon->SetItemData(NewItemData);
	DroppedWeapon->UpdateMesh();

	if (ItemData->GetItemData().ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("FirearmWeapon::Faild to load ItemID!!!!!!"));
	}

	// ������ �� �浹 Ȱ��ȭ
	if (USkeletalMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponMesh->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // ������ �ּ� ����
		WeaponMesh->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody); // ���� �ٵ� Ÿ�� ����
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block); // ��� ä�ο��� �浹 ó��
		WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn Ŭ���� �浹 ����
		WeaponMesh->SetEnableGravity(true); // �߷� ����
		// �÷��̾� �������� ������ ���� �߰� (������ ������)
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true);
	}

	// ���� ���� ����
	Destroy();
}

bool AFirearmWeapon::CanRelad()
{
	if (bIsEquipping || bIsReloading || bIsFiring)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::Begin_Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already reloading!"), *GetName());
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("%s reloading..."), *GetName());
}

void AFirearmWeapon::Reload()
{
	// ������ �� ź�� ����
	FirearmStats.AmmoCount = FirearmStats.MagazineCapacity;
	UE_LOG(LogTemp, Log, TEXT("%s reloaded. Ammo: %d/%d"), *GetName(), FirearmStats.AmmoCount, FirearmStats.MagazineCapacity);
}

void AFirearmWeapon::End_Reload()
{
	bIsReloading = false;
}

bool AFirearmWeapon::bIsInAim()
{
	return bIsAiming;
}

bool AFirearmWeapon::CanAim()
{
	if (bIsEquipping || bIsReloading || bIsAiming)
	{
		return false;
	}

	return true;
}

void AFirearmWeapon::AimDownSights()
{
	bIsAiming = true;
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: Aiming Down Sights!"));

	// TODO: ����

	return;
}

void AFirearmWeapon::StopAiming()
{
	if (!bIsAiming)
	{
		return;
	}

	bIsAiming = false;
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: %s stopped aiming"), *GetName());

	// TODO: ���� ����
	
	return;
}

int32 AFirearmWeapon::GetCurrentAmmo() const
{
	return FirearmStats.AmmoCount;
}

void AFirearmWeapon::SetWeaponData(const FWeaponItemData& NewWeaponData)
{
	Super::SetWeaponData(NewWeaponData);

	FirearmStats = NewWeaponData.FirearmStats;

	// ���� ���� ���� �ʱ�ȭ
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

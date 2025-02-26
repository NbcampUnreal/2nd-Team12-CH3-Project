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
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// �⺻ ������ �ּ� ���� (�ʿ��)
	SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);

	// �ʱ� ���� ����
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::Attack()
{
	// TODO : �߻� ����, �κ��丮�� ����
	if (CurrentAmmo <= 0 || bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo or reloading!"), *GetName());
		return;
	}

	// ��� ���·� ��ȯ
	bIsFiring = true;
	CurrentAmmo--;
	CurrentRecoil += FirearmStats.Recoil;  // �ݵ� ����

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), CurrentAmmo, CurrentRecoil);
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	/* ĳ������ �Ϻ� �Լ� �ʿ�
	// ������ ������ �ѱⰡ �ִٸ� ���� ����
	AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
	{
		EquippedWeapon->UnEquip(PlayerCharacter);
	}

	// �� �ѱ� ����
	PlayerCharacter->EquipWeapon(this);
	*/

	// ������ �� �浹 ��Ȱ��ȭ
	if (SkeletalMeshComponent)
	{
		// ���� �ùķ��̼� ��Ȱ��ȭ
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// �浹 ��Ȱ��ȭ (������ ���¿����� �浹�� ��Ȱ��ȭ)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// ������ ��ġ�� �̵�
	// PlayerCharacter->EquipWeapon(this); ���
	AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemData->GetItemData().AssetData.AttachSocketName);
	ItemState = EItemState::EIS_Equipped;
	UE_LOG(LogTemp, Log, TEXT("%s equipped by %s"), *GetName(), *PlayerCharacter->GetName());
}


void AFirearmWeapon::UnEquip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::UnEquip - PlayerCharacter is null!"));
		return;
	}

	// �÷��̾��� ���� ���� ���� ����
	// PlayerCharacter->UnEquipWeapon();
	UE_LOG(LogTemp, Log, TEXT("%s unequipped by %s"), *GetName(), *PlayerCharacter->GetName());

	// ���⸦ �ٴڿ� ���
	Drop(PlayerCharacter);
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
	DroppedWeapon->SetItemData(ItemData);

	// ������ �� �浹 Ȱ��ȭ
	if (USkeletalMeshComponent* WeaponMesh = DroppedWeapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponMesh->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
		WeaponMesh->SetCollisionObjectType(ECC_PhysicsBody); // ���� �ٵ� Ÿ�� ����
		WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block); // ��� ä�ο��� �浹 ó��

		// �÷��̾� �������� ������ ���� �߰� (������ ������)
		WeaponMesh->AddImpulse(PlayerCharacter->GetActorForwardVector() * 200.0f, NAME_None, true);
	}

	// ���� ���� ����
	ItemState = EItemState::EIS_World;
	Destroy();
}

void AFirearmWeapon::Reload()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is already reloading!"), *GetName());
		return;
	}

	bIsReloading = true;
	UE_LOG(LogTemp, Log, TEXT("%s reloading..."), *GetName());

	// ������ �� ź�� ����
	CurrentAmmo = FirearmStats.MagazineCapacity;
	bIsReloading = false;

	UE_LOG(LogTemp, Log, TEXT("%s reloaded. Ammo: %d/%d"), *GetName(), CurrentAmmo, FirearmStats.MagazineCapacity);
}

void AFirearmWeapon::AimDownSights()
{
	// TODO: ����
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: Aiming Down Sights!"));
}

void AFirearmWeapon::StopAiming()
{
	// TODO: ���� ����
	UE_LOG(LogTemp, Log, TEXT("FirearmWeapon: %s stopped aiming"), *GetName());
}

int32 AFirearmWeapon::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

void AFirearmWeapon::InitializeFirearmData(const FWeaponItemData& NewWeaponData)
{
	FirearmStats = ItemData->GetWeaponData().FirearmStats;

	// ���� ���� ���� �ʱ�ȭ
	CurrentAmmo = FirearmStats.MagazineCapacity;
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

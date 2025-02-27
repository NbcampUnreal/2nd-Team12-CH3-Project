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

	// �⺻ ������ �ּ� ���� (�ʿ��)
	if (ItemData)
	{
		SkeletalMeshComponent->SetSkeletalMesh(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
	}

	// �ʱ� ���� ����
	bIsReloading = false;
	bIsFiring = false;
	bIsAiming = false;
	CurrentRecoil = 0.0f;
}

void AFirearmWeapon::Attack()
{
	// TODO : �߻� ����, �κ��丮�� ����
	if (FirearmStats.AmmoCount <= 0 || bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is out of ammo or reloading!"), *GetName());
		return;
	}

	// ��� ���·� ��ȯ
	bIsFiring = true;
	FirearmStats.AmmoCount--;
	CurrentRecoil += FirearmStats.Recoil;  // �ݵ� ����

	UE_LOG(LogTemp, Log, TEXT("%s fired! Ammo left: %d, Recoil: %.2f"), *GetName(), FirearmStats.AmmoCount, CurrentRecoil);
}

void AFirearmWeapon::Equip(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirearmWeapon::Equip - PlayerCharacter is null!"));
		return;
	}

	// ������ ������ �ѱⰡ �ִٸ� ���� ����
	/*
	AWeaponBase* EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
	if (EquippedWeapon && EquippedWeapon->IsA(AFirearmWeapon::StaticClass()))
	{
		EquippedWeapon->UnEquip(PlayerCharacter);
	}
	*/
	// �� �ѱ� ����
	//PlayerCharacter->EquipWeapon(this);

	// ������ �� �浹 ��Ȱ��ȭ
	if (SkeletalMeshComponent)
	{
		// �⺻ �ּ� ����
		SkeletalMeshComponent->SetSkeletalMeshAsset(ItemData->GetItemData().AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset);
		SkeletalMeshComponent->SetVisibility(true);

		// ���� �ùķ��̼� ��Ȱ��ȭ
		SkeletalMeshComponent->SetSimulatePhysics(false);

		// �浹 ��Ȱ��ȭ (������ ���¿����� �浹�� ��Ȱ��ȭ)
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// ������ ��ġ�� �̵�
	// PlayerCharacter->EquipWeapon(this); ���
	FName SocketName = ItemData->GetItemData().AssetData.AttachSocketName;
	if (SocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Equip - No AttachSocketName set in ItemData, defaulting to 'hand_r'."));
		SocketName = TEXT("hand_r");
	}
	AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	
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

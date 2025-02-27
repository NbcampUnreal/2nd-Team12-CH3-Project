// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldWeapon.h"
#include "SPTPlayerCharacter.h"
#include "SPT/Items/Weapons/WeaponBase.h"
#include "SPT/Items/Weapons/FirearmWeapon.h"
// #include "SPT/Items/Weapons/MeleeWeapon.h"
// #include "SPT/Items/Weapons/ThrowableWeapon.h"
#include "Engine/World.h"

AWorldWeapon::AWorldWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// Skeletal Mesh �߰�
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	// ������ �ּ� ���� (���� �ùķ��̼� Ȱ��ȭ)
	if (ItemData && SkeletalMeshComponent)
	{
		if (ItemData->GetWeaponData().PhysicsAsset)
		{
			SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // ������ �ּ� ����
		}
		SkeletalMeshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
		SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody); // ���� �ٵ� Ÿ�� ����
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn Ŭ���� �浹 ����
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block); // ��� ä�ο��� �浹 ó��
		SkeletalMeshComponent->SetEnableGravity(true); // �߷� ����
		
		UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::SetEnableGravity!"));
	}
}

void AWorldWeapon::OnPickup(ASPTPlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldWeapon::OnPickup - PlayerCharacter is null!"));
		return;
	}

	// ���� �����Ͱ� ��������
	FItemData FoundItemData = ItemData->GetItemData();
	const FWeaponItemData& FoundWeaponData = ItemData->GetWeaponData();

	// ���� ������ ���� ������ Ŭ������ ����
	AWeaponBase* SpawnedWeapon = nullptr;
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	switch (FoundWeaponData.WeaponType)
	{
	case EWeaponType::EWT_Firearm:
		SpawnedWeapon = GetWorld()->SpawnActor<AFirearmWeapon>(AFirearmWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	case EWeaponType::EWT_Melee:
		// SpawnedWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(AMeleeWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	case EWeaponType::EWT_Throwable:
		// SpawnedWeapon = GetWorld()->SpawnActor<AThrowableWeapon>(AThrowableWeapon::StaticClass(), SpawnLocation, SpawnRotation);
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("AWorldWeapon::OnPickup - Unknown weapon type"));
		return;
	}

	if (!SpawnedWeapon)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn weapon!"));
		return;
	}

	// ���� ������ ����
	UItemDataObject* NewItemData = ItemData->CreateItemCopy();
	SpawnedWeapon->SetItemData(NewItemData);
	SpawnedWeapon->SetWeaponData(NewItemData->GetWeaponData());

	// ���� ���� (�� Ŭ�������� ���� ���� ����)
	SpawnedWeapon->Equip(PlayerCharacter);

	// ���� ���� ����
	Destroy();
	UE_LOG(LogTemp, Log, TEXT("Weapon picked up and equipped"));
}

#if WITH_EDITOR
void AWorldWeapon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? FName(*PropertyChangedEvent.Property->GetName()) : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AWorldWeapon, ItemID))
	{
		if (!ItemData)
		{
			ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
		}

		ItemData->InitializeItemData(ItemID);

		UpdateMesh();
		// ������ �ּ� ���� (���� �ùķ��̼� Ȱ��ȭ)
		if (SkeletalMeshComponent && ItemData->GetWeaponData().PhysicsAsset)
		{
			SkeletalMeshComponent->SetPhysicsAsset(ItemData->GetWeaponData().PhysicsAsset); // ������ �ּ� ����
			SkeletalMeshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ
			SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // �浹 Ȱ��ȭ
			SkeletalMeshComponent->SetCollisionObjectType(ECC_PhysicsBody); // ���� �ٵ� Ÿ�� ����
			SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Block); // ��� ä�ο��� �浹 ó��
			SkeletalMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // Pawn Ŭ���� �浹 ����
			SkeletalMeshComponent->SetEnableGravity(true); // �߷� ���� ä�ο��� �浹 ó��
		}

		UE_LOG(LogTemp, Log, TEXT("PostEditChangeProperty - ItemData updated for %s"), *ItemID.ToString());
	}
}

void AWorldWeapon::OnConstruction(const FTransform& Transform)
{
	if (!ItemDataTable || ItemID.IsNone())
	{
		UE_LOG(LogTemp, Log, TEXT("AWorldWeapon::OnConstruction: There is No ItemDataTable or ItemID for %s"), *GetName());
		return;
	}

	if (!ItemData)
	{
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());
	}

	ItemData->SetItemData(ItemData->GetItemData());
	UE_LOG(LogTemp, Log, TEXT("OnConstruction: Item data loaded for %s"), *GetName());

	// �޽� ������Ʈ
	UpdateMesh();
}
#endif

void AWorldWeapon::UpdateMesh()
{
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateMesh: ItemData is null!"));
		return;
	}

	// �޽� ������Ʈ ����
	const FItemData& FoundItemData = ItemData->GetItemData();
	const FWeaponItemData& FoundWeaponData = ItemData->GetWeaponData();

	// Static Mesh ������Ʈ
	if (StaticMeshComponent && FoundItemData.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(FoundItemData.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}

	// Skeletal Mesh ������Ʈ
	if (SkeletalMeshComponent && FoundItemData.AssetData.SkeletalMesh)
	{
		SkeletalMeshComponent->SetSkeletalMesh(FoundItemData.AssetData.SkeletalMesh);
		SkeletalMeshComponent->SetVisibility(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent is NULL or no mesh found for %s"), *GetName());
	}

	UE_LOG(LogTemp, Log, TEXT("UpdateMesh: Mesh updated!"));
}

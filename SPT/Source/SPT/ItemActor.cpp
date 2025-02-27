// Fill out your copyright notice in the Description page of Project Settings.

//임시용 아이템 액터 입니다. 정식으로 아이템이 추가될 시 바로 폐기하기

#include "ItemActor.h"
#include "Inventory/ItemData/EquipmentItem.h"
#include "Inventory/ItemData/ConsumableItemDataObject.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;		// 아이템이 회전하거나 하는 특수효과를 넣어야할 때 사용될수도 있다.


	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
	StaticMesh->SetSimulatePhysics(false);


	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.0f);
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	Collision->SetupAttachment(StaticMesh);


	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(StaticMesh);

	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidgetComponent->SetDrawSize(FVector2D(200, 50));
	PickupWidgetComponent->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprints/Inventory/UI/BP_PickUp"));
	if (WidgetClass.Succeeded())
	{
		PickupWidgetClass = WidgetClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Items/Weapons/FPS_Weapon_Bundle/Meshes/Accessories/SM_Vertgrip.SM_Vertgrip"));
	if (MeshAsset.Succeeded()) {
		StaticMesh->SetStaticMesh(MeshAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Items/Weapons/FPS_Weapon_Bundle/Materials/Accessories/M_V_Grip.M_V_Grip"));
	if (MaterialAsset.Succeeded()) {
		StaticMesh->SetMaterial(0, MaterialAsset.Object);
	}

	Scene->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));

	ItemName = "Can";
	ItemDescription = "This is just trash";
	ItemID = 0;
	ItemAmount = 1;
	static ConstructorHelpers::FObjectFinder<UTexture2D> IconTexture(TEXT("/Game/Fire.Fire"));
	if (IconTexture.Succeeded())
	{
		ItemIcon = IconTexture.Object;
	}

}



FString AItemActor::GetItemName()
{
	if (ItemData)
	{
		return ItemData->ItemName;
	}
	return FString("UnKnownItem");
}

int32 AItemActor::GetItemAmount()
{
	return int32();
}

UInventoryItem* AItemActor::GetItemData() const
{
	// 장비 아이템일 경우
	if (UEquipmentItem* EquipmentItem = Cast<UEquipmentItem>(ItemData))
	{
		return EquipmentItem;
	}

	// 소모품 아이템일 경우 (추가적으로 다른 아이템 처리 가능)
	// 어차피 장비아이템으로 취급할 것이기에  큰 의미는 없긴함
	if (UConsumableItemDataObject* ConsumableItem = Cast<UConsumableItemDataObject>(ItemData))
	{
		return ConsumableItem;
	}

	return ItemData; // 기본 아이템 반환
}

//void AItemActor::InitializeItem(UInventoryItem* ItemData)
//{
//	if (!ItemData) return;
//
//	// 기존 아이템 데이터 대신 참조를 저장
//	this->ItemData = ItemData;
//
//	// 필요하면 UI 갱신 등의 로직 추가
//}



void AItemActor::UseItem()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemActor : UseItem"));
}



void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPTPlayerCharacter* Player = Cast<ASPTPlayerCharacter>(OtherActor);
	if (Player && PickupWidget)
	{
		ShowPickupPrompt(true);
	}
}

void AItemActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASPTPlayerCharacter* Player = Cast<ASPTPlayerCharacter>(OtherActor);
	if (Player && PickupWidget)
	{
		ShowPickupPrompt(false);
	}
}

void AItemActor::ShowPickupPrompt(bool bShow)
{
	if (PickupWidgetComponent)
	{
		PickupWidgetComponent->SetVisibility(bShow);
	}
}



void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!ItemData)
	{
		// 기본적으로 장비 아이템 데이터 설정
		UEquipmentItem* EquipmentItem = NewObject<UEquipmentItem>(this);
		EquipmentItem->ItemName = FString("Sword");
		EquipmentItem->Description = "A powerful sword.";
		EquipmentItem->AttackPower = 50;
		EquipmentItem->DefensePower = 10;
		EquipmentItem->EquipmentSlot = FName("Weapon");
		EquipmentItem->ItemActorClass = AItemActor::StaticClass();
		EquipmentItem->ItemIcon = ItemIcon;

		ItemData = EquipmentItem;
	}

	if (PickupWidgetClass)
	{
		PickupWidget = CreateWidget<UUserWidget>(GetWorld(), PickupWidgetClass);
		if (PickupWidget)
		{
			PickupWidget->AddToViewport();
			PickupWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PickupWidgetClass is NULL!"));
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnOverlapEnd);

	Collision->SetCollisionObjectType(ECC_GameTraceChannel1);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


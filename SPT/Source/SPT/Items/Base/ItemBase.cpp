// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component ����
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// �ݸ��� ���� ����
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.0f);
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	Collision->SetupAttachment(StaticMeshComponent);

	// ������ �� �� �Լ� �۵�
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapEnd);

	// �ش� ä�γ��� ����
	Collision->SetCollisionObjectType(ECC_GameTraceChannel1);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	// ���� ����
	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(StaticMeshComponent);

	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidgetComponent->SetDrawSize(FVector2D(200, 50));
	PickupWidgetComponent->SetVisibility(false);

	// �⺻ ���´� ���� ����
	ItemState = EItemState::EIS_World;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is nullptr. Creating new ItemDataObject."));
		ItemData = NewObject<UItemDataObject>(this, UItemDataObject::StaticClass());

		if (!ItemData)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create ItemDataObject!"));
			return;  // �浹 ����
		}
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

	FName ItemID = ItemData->GetItemData().ItemID;
	if (ItemID.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemID is None! InitializeItemData skipped."));
	}
	else
	{
		ItemData->InitializeItemData(ItemID);
	}
}

void AItemBase::InitializeItem(FName ItemRowName)
{
	UE_LOG(LogTemp, Warning, TEXT("AItemBase : InitializeItem : run"));
	if (ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemBase : InitializeItem : play"));
		// ItemDataObject���� ������ �ʱ�ȭ
		ItemData->InitializeItemData(ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// �޽� ���� (�������Ʈ���� ������ �޽ð� ������ ����)
		if (LoadedItemData.AssetData.StaticMesh)
		{
			StaticMeshComponent->SetStaticMesh(LoadedItemData.AssetData.StaticMesh);
			StaticMeshComponent->SetVisibility(true);
		}
	}
}

void AItemBase::UpdateMeshForState(EItemState NewState)
{
	ItemState = NewState;

	if (!ItemData)
	{
		return;
	}

	const FItemData& Data = ItemData->GetItemData();
	// StaticMesh ���
	if (Data.AssetData.StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(Data.AssetData.StaticMesh);
		StaticMeshComponent->SetVisibility(true);
	}
}

AItemBase* AItemBase::CreateItemCopy() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateItemCopy failed: World is null."));
		return nullptr;
	}

	// ���� ������ transform�� �״�� ����Ͽ� ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItemBase* NewItem = World->SpawnActor<AItemBase>(GetClass(), GetActorTransform(), SpawnParams);
	if (NewItem && ItemData)
	{
		NewItem->SetItemData(ItemData->CreateItemCopy());
		NewItem->SetItemState(ItemState);
	}

	return NewItem;
}

EItemState AItemBase::GetItemState() const
{
	return ItemState;
}

void AItemBase::SetItemState(EItemState NewItemState)
{
	if (NewItemState != EItemState::EIS_MAX)
	{
		ItemState = NewItemState;
		UpdateMeshForState(NewItemState);
	}
}

UItemDataObject* AItemBase::GetItemData() const
{
	return ItemData;
}

void AItemBase::SetItemData(UItemDataObject* NewItemData)
{
	if (NewItemData)
	{
		ItemData = NewItemData;
		UE_LOG(LogTemp, Warning, TEXT("AItemBase::SetItemData called. Keeping existing item state."));
	}
}








UInventoryItem* AItemBase::GetItemInventoryData() const
{
	if (ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemBase : GetItemInventoryData : play"));
		// ItemDataObject�� ��ȿ�� ���, UItemDataObject���� UInventoryItem���� ��ȯ
		return ItemData->ConvertToInventoryItem();
	}

	return nullptr;
}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPTPlayerCharacter* Player = Cast<ASPTPlayerCharacter>(OtherActor);
	if (Player && PickupWidget)
	{
		ShowPickupPrompt(true);
	}
}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASPTPlayerCharacter* Player = Cast<ASPTPlayerCharacter>(OtherActor);
	if (Player && PickupWidget)
	{
		ShowPickupPrompt(false);
	}
}

void AItemBase::ShowPickupPrompt(bool bShow)
{
	if (PickupWidgetComponent)
	{
		PickupWidgetComponent->SetVisibility(bShow);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "SPTPlayerCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Static Mesh Component 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	// 콜리전 영역 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.0f);
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	Collision->SetupAttachment(StaticMeshComponent);

	// 오버랩 될 시 함수 작동
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapEnd);

	// 해당 채널끼리 적용
	Collision->SetCollisionObjectType(ECC_GameTraceChannel1);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 기본 상태는 월드 상태
	ItemState = EItemState::EIS_World;


	// 아이템 픽업 위젯을 할당하기 위해 추가한 코드입니다.
	// 위젯 생성
	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(StaticMeshComponent);

	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidgetComponent->SetDrawSize(FVector2D(200, 50));
	PickupWidgetComponent->SetVisibility(false);

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_PickUpWidget(TEXT("/Game/Blueprints/Inventory/UI/WBP_PickUpWidget"));
	if (WBP_PickUpWidget.Succeeded())
	{
		PickupWidgetClass = WBP_PickUpWidget.Class;
		PickupWidgetComponent->SetWidgetClass(PickupWidgetClass);
	}
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
			return;  // 충돌 방지
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
		// ItemDataObject에서 데이터 초기화
		ItemData->InitializeItemData(ItemRowName);

		const FItemData& LoadedItemData = ItemData->GetItemData();

		// 메시 설정 (블루프린트에서 설정된 메시가 있으면 적용)
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
	// StaticMesh 사용
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

	// 현재 액터의 transform을 그대로 사용하여 복제
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

		// 아이템 생성 시 인벤토리 데이터를 할당하기 위해 추가한 부분
        // 데이터가 제대로 설정되었는지 확인하고 초기화
        FName ItemID = ItemData->GetItemData().ItemID;
        if (!ItemID.IsNone())
        {
            // 아이템 ID가 유효한 경우 초기화
            ItemData->InitializeItemData(ItemID);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ItemID is None! InitializeItemData skipped."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SetItemData: NewItemData is nullptr!"));
    }
}






// 아이템 오버랩 관련 함수
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
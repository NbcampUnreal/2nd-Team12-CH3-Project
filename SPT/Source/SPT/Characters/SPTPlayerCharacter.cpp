// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"


////////////////////////////////////////////////////////////////////////////////
/* 아이템 디버깅 */
#include "SPT/Items/WorldItems/WorldItemActor.h"
#include "SPT/Items/Weapons/WeaponActor.h"
#include "SPT/Items/ConsumableItems/ConsumableItemActor.h"
#include "DrawDebugHelpers.h"


////////////////////////////////////////////////////////////////////////////////

ASPTPlayerCharacter::ASPTPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
    // 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp->bUsePawnControlRotation = true;
    // 앉기 시 카메라가 자연스럽게 이동하도록 카메라 렉 설정 및 설정 값
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 20.f;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);


    // 상호 작용 관련 변수 초기화
    InteractionCheckFrequency = 0.1f;
    InteractionCheckDistance = 250.0f;
    BaseEyeHeight = 80.0f;

    // 장착 아이템 메시 기본값 설정
    EquippedItem = nullptr;
    EquippedItemSocket = "hand_r";
}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    /* 아이템 디버깅 */
    if (GetWorld()->TimeSince(InteractionData.LastIneractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
}

void ASPTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (ASPTPlayerController* PlayerController = Cast<ASPTPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Move);
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartJump);

                EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopJump);
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Look);
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartSprint);
                EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopSprint);
            }

            if (PlayerController->CrouchAction)
            {
                EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartCrouch);
                EnhancedInput->BindAction(PlayerController->CrouchAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopCrouch);
            }

            if (PlayerController->ItemUseAction)
            {
                EnhancedInput->BindAction(PlayerController->ItemUseAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::ItemUse);
            }

            if (PlayerController->InteractAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartInteract);
            }

            if (PlayerController->InventoryAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::OnOffInventory);
            }

            if (PlayerController->ReloadAction)
            {
                // 키 입력 중에 1번만 호출됨
                EnhancedInput->BindAction(PlayerController->ReloadAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartReload);
            }
        }
    }
}

bool ASPTPlayerCharacter::EquipItem(AWorldItemActor* NewItem)
{
    if (!NewItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipItem: No item provided"));
        return false;
    }

    // 기존 무기 해제
    if (EquippedItem)
    {
        UnEquipItem();
    }

    // 현재 장착한 아이템 저장
    EquippedItem = NewItem;

    FName AttachSocket = NewItem->GetItemData().AssetData.AttachSocketName;
    if (AttachSocket.IsNone())
    {
        AttachSocket = EquippedItemSocket;
    }

    // 아이템 장착
    EquippedItem->AttachToComponent(
        GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocket);

    // 오프셋 적용
    FItemData ItemData = NewItem->GetItemData();
    if (UPrimitiveComponent* MeshComp = EquippedItem->GetMeshComponent())
    {
        MeshComp->SetRelativeLocation(ItemData.AssetData.EquipPositionOffset);
        MeshComp->SetRelativeRotation(ItemData.AssetData.EquipRotationOffset);
    }

    UE_LOG(LogTemp, Log, TEXT("EquipItem: Equipped %s"), *NewItem->GetName());
    return true;
}

bool ASPTPlayerCharacter::UnEquipItem()
{
    if (!EquippedItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("UnEquipItem: No item equipped"));
        return false;
    }

    // 현재 장착된 아이템의 정보 가져오기
    FItemData ItemData = EquippedItem->GetItemData();
    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        DropEquippedItem();
        UE_LOG(LogTemp, Log, TEXT("UnEquipItem: Item unequipped"));
    }
    else
    {
        /* 인벤토리 추가
         * 총기 아이템 및 근접 무기 아이템이 아닌 다른 아이템의 경우 장비 해제 시 인벤토리로 들어갑니다.
         * 투척류 무기, 소모품(힐템 등) UItemDataObject를 통하여 아이템 정보를 얻은 후
         * 인벤토리에 넣으면 됩니다.
         * 수량에 대한 로직은 조금 더 생각해봐야할 거 같습니다.
         * 
        if (InventoryComponent)
        {
            UItemDataObject* NewItem = NewObject<UItemDataObject>();
            if (NewItem)
            {
                NewItem->SetItemData(ItemData);
                InventoryComponent->AddItem(NewItem);
                UE_LOG(LogTemp, Log, TEXT("UnEquipItem: %s added to inventory"), *ItemData.TextData.Name.ToString());
            }
        }
        */

        EquippedItem->Destroy();
        UE_LOG(LogTemp, Log, TEXT("UnEquipItem: Item moved to inventory"));
    }

    EquippedItem = nullptr;
    return true;
}

void ASPTPlayerCharacter::DropEquippedItem()
{
    if (!EquippedItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("DropEquippedItem: No equipped item to drop"));
        return;
    }

    EquippedItem->OnDrop(this);
    UE_LOG(LogTemp, Log, TEXT("DropEquippedItem: Dropped equipped item with state retained"));
}

void ASPTPlayerCharacter::DropItem()
{
    /* 인벤토리 추가
     * 총기 아이템 및 근접 무기 아이템이 아닌 다른 아이템의 경우 장비 해제 시 인벤토리로 들어갑니다.
     * 투척류 무기, 소모품(힐템 등) UItemDataObject를 통하여 아이템 정보를 얻은 후
     * 인벤토리에 넣으면 됩니다.
     * 수량에 대한 로직은 조금 더 생각해봐야할 거 같습니다.
     *
    if (InventoryComponent)
    {
        UItemDataObject* NewItem = NewObject<UItemDataObject>();
        if (NewItem)
        {
            NewItem->SetItemData(ItemData);
            InventoryComponent->AddItem(NewItem);
            UE_LOG(LogTemp, Log, TEXT("UnEquipItem: %s added to inventory"), *ItemData.TextData.Name.ToString());
        }
    }
    */
}

void ASPTPlayerCharacter::UseConsumable()
{
    if (!EquippedItem || EquippedItem->GetItemData().ItemType != EItemType::EIT_Consumable)
    {
        UE_LOG(LogTemp, Warning, TEXT("UseConsumable: No consumable item equipped"));
        return;
    }

    AConsumableItemActor* Consumable = Cast<AConsumableItemActor>(EquippedItem);
    if (!Consumable)
    {
        UE_LOG(LogTemp, Warning, TEXT("UseConsumable: Equipped item is not a consumable"));
        return;
    }

    // TODO: 사용 애니메이션 실행
    /*
   if (Consumable->GetItemData().AnimationData.UseAnimation && GetMesh()->GetAnimInstance())
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        float AnimationDuration = AnimInstance->Montage_Play(Consumable->GetItemData().AnimationData.UseAnimation);

        // 애니메이션이 끝난 후 아이템 사용 처리
        GetWorldTimerManager().SetTimer(
            TimerHandle_ItemUse,
            [this, Consumable]()
            {
                if (Consumable)
                {
                    Consumable->Use(this);
                }
            },
            AnimationDuration,
            false
        );
    }
    else
    {
        // 애니메이션이 없으면 바로 아이템 사용
        Consumable->Use(this);
    }
    */

    UE_LOG(LogTemp, Log, TEXT("UseConsumable: %s used"), *Consumable->GetName());
}

AWorldItemActor* ASPTPlayerCharacter::GetEquippedItem() const
{
    return EquippedItem;
}

void ASPTPlayerCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void ASPTPlayerCharacter::Look(const FInputActionValue& value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();

    // 좌우 회전
    AddControllerYawInput(LookInput.X);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y);
}

void ASPTPlayerCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ASPTPlayerCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ASPTPlayerCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASPTPlayerCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ASPTPlayerCharacter::StartCrouch(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Crouch();
    }
}

void ASPTPlayerCharacter::StopCrouch(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        UnCrouch();
    }
}

void ASPTPlayerCharacter::ItemUse(const FInputActionValue& value)
{
    // 현재 장착중인 아이템을 사용한다.
    if (value.Get<bool>())
    {
        if (EquippedItem)
        {
            EquippedItem->Use(this);
        }
    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // 라인트레이스를 통해 찾았던 액터와 상호작용 한다.
    if (value.Get<bool>())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->Interact(this);
        }
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // 인벤토리를 켜거나 끈다.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // 현재 착용중인 아이템이 총이거나 재장전 할 수 있는 아이템이라면 재장전
    if (value.Get<bool>())
    {
        
    }
}

////////////////////////////////////////////////////////////////////////////////
void ASPTPlayerCharacter::PerformInteractionCheck()
{
    InteractionData.LastIneractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector PlayerViewLocation = GetPawnViewLocation();
    FVector PlayerViewDirection = GetViewRotation().Vector();
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    // 라인 트레이스로 아이템 감지
    FHitResult LineTraceHit;
    bool bLineHit = GetWorld()->LineTraceSingleByChannel(
        LineTraceHit,
        PlayerViewLocation,
        PlayerViewLocation + (PlayerViewDirection * InteractionCheckDistance * 1.5f),
        ECC_Visibility,
        QueryParams
    );

    if (bLineHit)
    {
        AWorldItemActor* LineHitItem = Cast<AWorldItemActor>(LineTraceHit.GetActor());
        if (LineHitItem && LineHitItem->Implements<UInteractableInterface>())
        {
            FoundInteractable(LineHitItem);
        }
    }

    // 스피어 트레이스로 근처 아이템 감지
    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResults,
        PlayerViewLocation,
        PlayerViewLocation + (PlayerViewDirection * InteractionCheckDistance),
        FQuat::Identity,
        ECC_Visibility,
        FCollisionShape::MakeSphere(InteractionCheckDistance * 0.6f),
        QueryParams
    );

    AWorldItemActor* BestCandidate = nullptr;
    float BestDotProduct = 0.7f;

    for (FHitResult Hit : HitResults)
    {
        AWorldItemActor* Item = Cast<AWorldItemActor>(Hit.GetActor());
        if (!Item || !Item->GetMeshComponent() || !Item->Implements<UInteractableInterface>())
        {
            continue;
        }

        FVector DirectionToItem = (Item->GetActorLocation() - PlayerViewLocation).GetSafeNormal();
        float DotProduct = FVector::DotProduct(PlayerViewDirection, DirectionToItem);

        // 플레이어가 바라보는 방향(60도 이내)만 필터링
        if (DotProduct > BestDotProduct)
        {
            BestDotProduct = DotProduct;
            BestCandidate = Item;
        }
    }

    // 최적 아이템 선택
    if (BestCandidate)
    {
        if (BestCandidate != InteractionData.CurrentInteractable)
        {
            FoundInteractable(BestCandidate);
        }
    }
    else
    {
        NoInteractableFound();
    }
}

void ASPTPlayerCharacter::FoundInteractable(AActor* NewInteractable)
{
    if (IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }

    // 기존 포커스 종료
    if (InteractionData.CurrentInteractable)
    {
        TargetInteractable = InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus();
    }

    // 새로운 포커스 설정
    InteractionData.CurrentInteractable = NewInteractable;
    TargetInteractable = NewInteractable;

    TargetInteractable->BeginFocus();
}

void ASPTPlayerCharacter::NoInteractableFound()
{
    if (IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }

    if (InteractionData.CurrentInteractable)
    {
        // 상호 작용 후 월드에서 아이템이 사라졌을 때 Crash 방지
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->EndFocus();
        }

        // hide interaction widget on the HUD

        InteractionData.CurrentInteractable = nullptr;
        TargetInteractable = nullptr;
    }
}

void ASPTPlayerCharacter::BeginInteract()
{
    // verify nothing has changed with the interactable state since beginning interaction
    PerformInteractionCheck();

    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->BeginInteract();

            if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
            {
                // Interact();
            }
            else
            {
                GetWorldTimerManager().SetTimer(
                    TimerHandle_Interaction,
                    this,
                    &ASPTPlayerCharacter::Interact,
                    TargetInteractable->InteractableData.InteractionDuration,
                    false);
            }
        }
    }
}

void ASPTPlayerCharacter::EndInteract()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->EndInteract();
    }
}

void ASPTPlayerCharacter::Interact()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact(this);
    }
}


bool ASPTPlayerCharacter::IsInteracting() const
{
    return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction);
}

////////////////////////////////////////////////////////////////////////////////

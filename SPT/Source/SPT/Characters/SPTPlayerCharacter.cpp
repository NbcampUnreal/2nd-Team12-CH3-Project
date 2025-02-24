// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"


////////////////////////////////////////////////////////////////////////////////
/* ������ ����� */
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
    // ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
    SpringArmComp->bUsePawnControlRotation = true;
    // �ɱ� �� ī�޶� �ڿ������� �̵��ϵ��� ī�޶� �� ���� �� ���� ��
    SpringArmComp->bEnableCameraLag = true;
    SpringArmComp->CameraLagSpeed = 20.f;
    SpringArmComp->bEnableCameraRotationLag = true;
    SpringArmComp->CameraRotationLagSpeed = 20.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);


    // ��ȣ �ۿ� ���� ���� �ʱ�ȭ
    InteractionCheckFrequency = 0.1f;
    InteractionCheckDistance = 250.0f;
    BaseEyeHeight = 80.0f;

    // ���� ������ �޽� �⺻�� ����
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

    /* ������ ����� */
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
                // Ű �Է� �߿� 1���� ȣ���
                EnhancedInput->BindAction(PlayerController->InteractAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::StartInteract);
            }

            if (PlayerController->InventoryAction)
            {
                // Ű �Է� �߿� 1���� ȣ���
                EnhancedInput->BindAction(PlayerController->InventoryAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::OnOffInventory);
            }

            if (PlayerController->ReloadAction)
            {
                // Ű �Է� �߿� 1���� ȣ���
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

    // ���� ���� ����
    if (EquippedItem)
    {
        UnEquipItem();
    }

    // ���� ������ ������ ����
    EquippedItem = NewItem;

    FName AttachSocket = NewItem->GetItemData().AssetData.AttachSocketName;
    if (AttachSocket.IsNone())
    {
        AttachSocket = EquippedItemSocket;
    }

    // ������ ����
    EquippedItem->AttachToComponent(
        GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        AttachSocket);

    // ������ ����
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

    // ���� ������ �������� ���� ��������
    FItemData ItemData = EquippedItem->GetItemData();
    if (ItemData.ItemType == EItemType::EIT_Weapon)
    {
        DropEquippedItem();
        UE_LOG(LogTemp, Log, TEXT("UnEquipItem: Item unequipped"));
    }
    else
    {
        /* �κ��丮 �߰�
         * �ѱ� ������ �� ���� ���� �������� �ƴ� �ٸ� �������� ��� ��� ���� �� �κ��丮�� ���ϴ�.
         * ��ô�� ����, �Ҹ�ǰ(���� ��) UItemDataObject�� ���Ͽ� ������ ������ ���� ��
         * �κ��丮�� ������ �˴ϴ�.
         * ������ ���� ������ ���� �� �����غ����� �� �����ϴ�.
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
    /* �κ��丮 �߰�
     * �ѱ� ������ �� ���� ���� �������� �ƴ� �ٸ� �������� ��� ��� ���� �� �κ��丮�� ���ϴ�.
     * ��ô�� ����, �Ҹ�ǰ(���� ��) UItemDataObject�� ���Ͽ� ������ ������ ���� ��
     * �κ��丮�� ������ �˴ϴ�.
     * ������ ���� ������ ���� �� �����غ����� �� �����ϴ�.
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

    // TODO: ��� �ִϸ��̼� ����
    /*
   if (Consumable->GetItemData().AnimationData.UseAnimation && GetMesh()->GetAnimInstance())
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        float AnimationDuration = AnimInstance->Montage_Play(Consumable->GetItemData().AnimationData.UseAnimation);

        // �ִϸ��̼��� ���� �� ������ ��� ó��
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
        // �ִϸ��̼��� ������ �ٷ� ������ ���
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
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = value.Get<FVector2D>();

    // �¿� ȸ��
    AddControllerYawInput(LookInput.X);
    // ���� ȸ��
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
    // ���� �������� �������� ����Ѵ�.
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
    // ����Ʈ���̽��� ���� ã�Ҵ� ���Ϳ� ��ȣ�ۿ� �Ѵ�.
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
    // �κ��丮�� �Ѱų� ����.
    if (value.Get<bool>())
    {

    }
}

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // ���� �������� �������� ���̰ų� ������ �� �� �ִ� �������̶�� ������
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

    // ���� Ʈ���̽��� ������ ����
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

    // ���Ǿ� Ʈ���̽��� ��ó ������ ����
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

        // �÷��̾ �ٶ󺸴� ����(60�� �̳�)�� ���͸�
        if (DotProduct > BestDotProduct)
        {
            BestDotProduct = DotProduct;
            BestCandidate = Item;
        }
    }

    // ���� ������ ����
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

    // ���� ��Ŀ�� ����
    if (InteractionData.CurrentInteractable)
    {
        TargetInteractable = InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus();
    }

    // ���ο� ��Ŀ�� ����
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
        // ��ȣ �ۿ� �� ���忡�� �������� ������� �� Crash ����
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

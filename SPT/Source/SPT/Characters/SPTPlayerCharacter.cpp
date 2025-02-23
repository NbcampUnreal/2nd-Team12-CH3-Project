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
    InteractionCheckDistance = 225.0f;
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

    if (EquippedItem)
    {
        UnEquipItem();
    }

    // ���ο� �������� �տ� ����
    EquippedItem = NewItem;
    EquippedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, EquippedItemSocket);
    EquippedItem->UpdateItemState(EItemState::EIS_Equipped);

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

    // ������ ���
    EquippedItem->OnDrop(this);
    EquippedItem = nullptr;

    /* ������ ���� �� �κ��丮
    EquippedItem->Destroy();
    EquippedItem = nullptr;
    */

    UE_LOG(LogTemp, Log, TEXT("UnEquipItem: Item unequipped and moved to inventory"));
    return true;
}

void ASPTPlayerCharacter::DropItem()
{
    if (!EquippedItem)
    {
        UE_LOG(LogTemp, Warning, TEXT("DropItem: No item to drop"));
        return;
    }

    EquippedItem->OnDrop(this);
    EquippedItem = nullptr;
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
    if (Consumable->UseAnimation && GetMesh()->GetAnimInstance())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(Consumable->UseAnimation);
    }
    */

    // ������ ȿ�� ����
    Consumable->Use(this);

    // ��� �� ������ ����
    Consumable->Destroy();
    EquippedItem = nullptr;

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

////////////////////////////////////////////////////////////////////////////////
void ASPTPlayerCharacter::PerformInteractionCheck()
{
    InteractionData.LastIneractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector TraceStart{ GetPawnViewLocation() };    // Pawn ���� �þ� ���� ����
    FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };   // Pawn�� �ٶ󺸴� ���⿡�� ��ȣ �ۿ� �ִ� �Ÿ�����

    float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());
    // �ʿ��� ��쿡 ���� ���� (���̳� �ڸ� �� ��� ������°�?�� Ȯ���غ�����)
    if (LookDirection > 0)
    {
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f); // ���� Ʈ���̽� Ȯ���� ���� ����� ����

        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        FHitResult TraceHit;

        if (GetWorld()->LineTraceSingleByChannel(
            TraceHit,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            QueryParams))
        {
            if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
            {
                const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();

                if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
                {
                    FoundInteractable(TraceHit.GetActor());
                    return;
                }

                if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
                {
                    return;
                }
            }
        }
    }

    NoInteractableFound();      // ��ȣ �ۿ� ���� �������� �ƴ� ��
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

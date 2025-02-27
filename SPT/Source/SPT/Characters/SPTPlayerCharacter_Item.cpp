// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"
///////////////////////////////////////////////////////////////////////

#include "SPT/Items/Worlds/WorldItemBase.h"
#include "SPT/Items/Weapons/WeaponBase.h"
// #include "SPT/Items/Consumables/ConsumableItem.h" // �ʿ�� �߰�

///////////////////////////////////////////////////////////////////////


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

    /* ��ȣ �ۿ� �߰� */
    InteractionCheckFrequency = 0.1f;
    InteractionCheckDistance = 250.0f;
}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ASPTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
}

///////////////////////////////////////////////////////////////////////

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
        EquippedWeapon->PrimaryAction(this);  //// �߰�
    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // ����Ʈ���̽��� ���� ã�Ҵ� ���Ϳ� ��ȣ�ۿ� �Ѵ�.
    if (value.Get<bool>())
    {
        Interact(); //// �߰�
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ASPTPlayerCharacter::EquipWeapon(AWeaponBase* NewItem)
{
    /* ���� �ʿ� (��� X)
     * ���� ������ ���, ��ȣ�ۿ� (Pickup)�� �� ���
     * �� ������ ������ ���� ���� ������ �޶�
     * WeaponBase Ȥ�� FirearmWeapon ���� ���� ���� Ŭ�������� AttachToComponent�� �����Ǿ� �ֽ��ϴ�.
     */
    if (!NewItem) return false;
    if (EquippedWeapon)
    {
        UnEquipWeapon();
    }

    EquippedWeapon = NewItem;

    if (!EquippedWeapon)
    {
        return false;
    }

    // EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, );
    return true;
}

bool ASPTPlayerCharacter::UnEquipWeapon()
{
    /* ���� �ʿ� (��� ��)
     * ���� ���� ���� Ű�� ������, ���⸦ ������ ä �ٸ� �ѱ���� Pickup�� ��� ȣ��˴ϴ�.
     * ��� �ٷ� ���⿡�� UnEquip�ص� �Ǳ⿡ ���Ŀ� �Һ�ǰ�� ����, �����ϴ� ���� ���� �� ����� �� �ֽ��ϴ�.
     */
    if (EquippedWeapon) {
        EquippedWeapon->UnEquip(this);
        EquippedWeapon = nullptr;
    }

    return false;
}

void ASPTPlayerCharacter::DropWeapon()
{
    if (EquippedWeapon)
    {
        EquippedWeapon->Drop(this);
    }
}

AWeaponBase* ASPTPlayerCharacter::GetEquippedWeapon() const
{
    return EquippedWeapon;
}


/* �Ʒ��� �Լ����ʹ� ��ȣ�ۿ��� ���� �Լ��Դϴ�.
 * �켱 ���� Ʈ���̽��� �ٶ󺸴� ������ �������� �켱 �����Ǹ�,
 * ���Ǿ� Ʈ���̽��� ��ó�� �ִ� ���� �þ߰� �̳��� ������ ���� �����˴ϴ�.
 *
 * Tick �Լ����� ������ ��ȣ�ۿ� �˻�(LastInteractionCheckTime) ����
 * ���� �ð��� (InteractionCheckFrequency ��) ������
 * PerformInteractionCheck()�� ȣ���Ͽ� ���ο� ��ȣ�ۿ� ������ �����մϴ�.
 */

void ASPTPlayerCharacter::PerformInteractionCheck()
{
    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

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
        AWorldItemBase* LineHitItem = Cast<AWorldItemBase>(LineTraceHit.GetActor());
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

    AWorldItemBase* BestCandidate = nullptr;
    float BestDotProduct = 0.7f;

    for (FHitResult Hit : HitResults)
    {
        AWorldItemBase* Item = Cast<AWorldItemBase>(Hit.GetActor());
        if (!Item || !Item->Implements<UInteractableInterface>())
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

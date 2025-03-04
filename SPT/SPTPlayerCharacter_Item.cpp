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
// #include "SPT/Items/Consumables/ConsumableItem.h" // 필요시 추가

///////////////////////////////////////////////////////////////////////


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

    /* 상호 작용 추가 */
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
        EquippedWeapon->PrimaryAction(this);  //// 추가
    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // 라인트레이스를 통해 찾았던 액터와 상호작용 한다.
    if (value.Get<bool>())
    {
        Interact(); //// 추가
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ASPTPlayerCharacter::EquipWeapon(AWeaponBase* NewItem)
{
    /* 수정 필요 (사용 X)
     * 현재 무기의 경우, 상호작용 (Pickup)을 할 경우
     * 각 무기의 종류에 따라 장착 로직이 달라
     * WeaponBase 혹은 FirearmWeapon 같은 무기 하위 클래스에서 AttachToComponent가 구현되어 있습니다.
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
    /* 수정 필요 (사용 중)
     * 아직 장착 해제 키는 없지만, 무기를 장착한 채 다른 총기류를 Pickup할 경우 호출됩니다.
     * 사실 바로 무기에서 UnEquip해도 되기에 추후에 소비품을 장착, 해제하는 로직 구현 시 변경될 수 있습니다.
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


/* 아래의 함수부터는 상호작용을 위한 함수입니다.
 * 우선 라인 트레이스로 바라보는 방향의 아이템이 우선 감지되며,
 * 스피어 트레이스로 근처에 있는 일정 시야각 이내의 아이템 역시 감지됩니다.
 *
 * Tick 함수에서 마지막 상호작용 검사(LastInteractionCheckTime) 이후
 * 일정 시간이 (InteractionCheckFrequency 초) 지나면
 * PerformInteractionCheck()를 호출하여 새로운 상호작용 감지를 수행합니다.
 */

void ASPTPlayerCharacter::PerformInteractionCheck()
{
    InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

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
        AWorldItemBase* LineHitItem = Cast<AWorldItemBase>(LineTraceHit.GetActor());
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"
#include "EquipmentInventory.h"
#include "ConsumableInventory.h"
#include "InventoryManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"     // ĳ���� ������ ĸ�Ŀ뵵 �Դϴ�.
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "SPTPlayerController.h"

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
    SpringArmComp->SocketOffset = FVector(0.f, 75.f, 100.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
}

void ASPTPlayerCharacter::AddItemToInventory(AItemActor* Item)
{
    // �������� ��ȿ���� Ȯ��
    if (!Item)
    {
        return;
    }

    // ������ �Ŵ����� ��ȿ���� Ȯ��
    if (InventoryManager)
    {
        // �������� ������ �Ŵ����� �߰�
        InventoryManager->AddItemToInventory(Item->GetItemData());
    }
}

void ASPTPlayerCharacter::ToggleInventory()
{
    if (InventoryMainWidgetInstance)
    {
        bool bIsVisible = InventoryMainWidgetInstance->IsVisible();
        InventoryMainWidgetInstance->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

        if (!bIsVisible)
        {
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (PlayerController)
            {
                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(InventoryMainWidgetInstance->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
                //PlayerController->SetPause(true);
            }
        }
        else
        {
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (PlayerController)
            {
                PlayerController->SetInputMode(FInputModeGameOnly());
                PlayerController->bShowMouseCursor = false;
                //PlayerController->SetPause(false);
            }
        }
    }
}

void ASPTPlayerCharacter::TryPickupItem()
{
    FVector Start = GetActorLocation();
    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 200.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GameTraceChannel1, Params);


    if (bHit)
    {
        AItemActor* ItemActor = Cast<AItemActor>(HitResult.GetActor());
        if (ItemActor && InventoryManager)
        {
            UInventoryItem* ItemData = ItemActor->GetItemData();
            if (ItemData)
            {
                InventoryManager->AddItemToInventory(ItemData);

                ItemActor->Destroy();
            }
        }
    }
}

void ASPTPlayerCharacter::DropItem(UInventoryItem* InventoryItem)
{
    if (InventoryManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("SPTPlayerCharacter : DropItem"));

        FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
        InventoryManager->DropItem(InventoryItem, DropLocation);
    }
}

AInventoryManager* ASPTPlayerCharacter::GetInventory() const
{
    return InventoryManager;
}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    // InventoryManagerClass�� �Ҵ�Ǿ����� Ȯ��
    if (InventoryManagerClass)
    {
        // �κ��丮 �Ŵ��� ����
        InventoryManager = GetWorld()->SpawnActor<AInventoryManager>(InventoryManagerClass);
        if (InventoryManager)
        {
            if (EquipmentInventoryClass)
            {
                EquipmentInventory = GetWorld()->SpawnActor<AEquipmentInventory>(EquipmentInventoryClass);
                InventoryManager->RegisterInventory(EquipmentInventory);
            }

            if (ConsumableInventoryClass)
            {
                ConsumableInventory = GetWorld()->SpawnActor<AConsumableInventory>(ConsumableInventoryClass);
                InventoryManager->RegisterInventory(ConsumableInventory);
            }
        }
    }

    if (InventoryMainWidgetClass)
    {
        InventoryMainWidgetInstance = CreateWidget<UInventoryMainWidget>(GetWorld(), InventoryMainWidgetClass);
        if (InventoryMainWidgetInstance)
        {
            InventoryMainWidgetInstance->AddToViewport();
            InventoryMainWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (InventoryManager)
    {
        InventoryManager->SetInventoryWidget(InventoryMainWidgetInstance);
    }
}

void ASPTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Started, this, &ASPTPlayerCharacter::StartMove);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ASPTPlayerCharacter::Move);
                EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &ASPTPlayerCharacter::StopMove);
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

void ASPTPlayerCharacter::StartMove(const FInputActionValue& value)
{
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bUseControllerDesiredRotation = true;
    }
}

void ASPTPlayerCharacter::Move(const FInputActionValue& value)
{
    if (Controller)
    {
        const FVector2D MoveInput = value.Get<FVector2D>();
        const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

        if (MoveInput.X != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
            AddMovementInput(MovementDirection, MoveInput.X);
        }

        if (MoveInput.Y != 0.0f)
        {
            const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
            AddMovementInput(MovementDirection, MoveInput.Y);
        }
    }
}

void ASPTPlayerCharacter::StopMove(const FInputActionValue& value)
{
    if (UCharacterMovementComponent* CharacterMovementComp = GetCharacterMovement())
    {
        CharacterMovementComp->bUseControllerDesiredRotation = false;
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
        
    }
}

void ASPTPlayerCharacter::StartInteract(const FInputActionValue& value)
{
    // ����Ʈ���̽��� ���� ã�Ҵ� ���Ϳ� ��ȣ�ۿ� �Ѵ�.
    if (value.Get<bool>())
    {
        TryPickupItem();
    }
}

void ASPTPlayerCharacter::OnOffInventory(const FInputActionValue& value)
{
    // �κ��丮�� �Ѱų� ����.
    if (value.Get<bool>())
    {
        if (InventoryMainWidgetInstance)
        {
            bool bIsVisible = InventoryMainWidgetInstance->IsVisible();
            InventoryMainWidgetInstance->SetVisibility(bIsVisible ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

            if (!bIsVisible)
            {
                APlayerController* PlayerController = Cast<APlayerController>(GetController());
                if (PlayerController)
                {
                    FInputModeGameAndUI InputMode;
                    InputMode.SetWidgetToFocus(InventoryMainWidgetInstance->TakeWidget());
                    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                    PlayerController->SetInputMode(InputMode);
                    PlayerController->bShowMouseCursor = true;
                    //PlayerController->SetPause(true);
                }
            }
            else
            {
                APlayerController* PlayerController = Cast<APlayerController>(GetController());
                if (PlayerController)
                {
                    PlayerController->SetInputMode(FInputModeGameOnly());
                    PlayerController->bShowMouseCursor = false;
                    //PlayerController->SetPause(false);
                }
            }
        }
    }
}

void ASPTPlayerCharacter::StartReload(const FInputActionValue& value)
{
    // ���� �������� �������� ���̰ų� ������ �� �� �ִ� �������̶�� ������
    if (value.Get<bool>())
    {
        
    }
}


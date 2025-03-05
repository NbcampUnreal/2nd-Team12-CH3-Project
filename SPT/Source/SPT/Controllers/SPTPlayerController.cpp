// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainHUD.h"


ASPTPlayerController::ASPTPlayerController()
{
	IMC = nullptr;
	MoveAction = nullptr;
	LookAction = nullptr;
	JumpAction = nullptr;
	SprintAction = nullptr;
    AimingAction = nullptr;
    AttackAction = nullptr;
    ToggleAutoFire = nullptr;
}

void ASPTPlayerController::BeginPlay()
{
    // ���� �߰�
    AddMapping();

    // PlayerMainHUD ���� �� ����Ʈ �߰�
    ShowPlayerMainHUD();

    if (ABaseCharacter* CurChar = GetPawn<ABaseCharacter>())
    {
        CurChar->OnDeathDelegate.AddDynamic(this, &ASPTPlayerController::RemoveMapping);
    }
}

void ASPTPlayerController::AddMapping()
{
    // ���� �߰�
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC)
            {
                // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
                // �켱����(Priority)�� 0�� ���� ���� �켱����
                Subsystem->AddMappingContext(IMC, 0);
            }
        }
    }
}

void ASPTPlayerController::RemoveMapping()
{
    // ��� ���� ����
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->ClearAllMappings();
        }
    }
}

void ASPTPlayerController::ShowPlayerMainHUD()
{
    // PlayerMainHUD ���� �� ����Ʈ �߰�
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UPlayerMainHUD>(this, HUDWidgetClass);

        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
            
            if (ABaseCharacter* CurChar = GetPawn<ABaseCharacter>())
            {
                // ü�� ����� ������Ʈ �Լ��� ȣ��ǵ��� ��������Ʈ
                CurChar->OnHealthChangedDelegate.AddDynamic(HUDWidgetInstance, &UPlayerMainHUD::HPUpdate);

                // �÷��̾� ĳ���� ����� 
                CurChar->OnDeathDelegate.AddDynamic(HUDWidgetInstance, &UPlayerMainHUD::HidePlayUI);
                CurChar->OnDeathDelegate.AddDynamic(HUDWidgetInstance, &UPlayerMainHUD::ShowDeathUI);
            }

            SetShowMouseCursor(false);
            SetInputMode(FInputModeGameOnly());
        }
    }
}

UPlayerMainHUD* ASPTPlayerController::GetHUDWidgetInstance() const
{
    return HUDWidgetInstance;
}

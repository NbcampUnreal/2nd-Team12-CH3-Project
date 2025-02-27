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
}

void ASPTPlayerController::BeginPlay()
{
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

    ShowPlayerMainHUD();
}

void ASPTPlayerController::ShowPlayerMainHUD()
{
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UPlayerMainHUD>(this, HUDWidgetClass);

        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
            
            if (ABaseCharacter* CurChar = GetPawn<ABaseCharacter>())
            {
                CurChar->OnHealthChangedDelegate.AddDynamic(HUDWidgetInstance, &UPlayerMainHUD::HPUpdate);
            }

            SetShowMouseCursor(false);
            SetInputMode(FInputModeGameOnly());
        }
    }
}
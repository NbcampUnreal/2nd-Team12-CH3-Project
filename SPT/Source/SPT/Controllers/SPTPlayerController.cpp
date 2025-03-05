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
    // 매핑 추가
    AddMapping();

    // PlayerMainHUD 생성 및 뷰포트 추가
    ShowPlayerMainHUD();

    if (ABaseCharacter* CurChar = GetPawn<ABaseCharacter>())
    {
        CurChar->OnDeathDelegate.AddDynamic(this, &ASPTPlayerController::RemoveMapping);
    }
}

void ASPTPlayerController::AddMapping()
{
    // 매핑 추가
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (IMC)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(IMC, 0);
            }
        }
    }
}

void ASPTPlayerController::RemoveMapping()
{
    // 모든 매핑 제거
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
    // PlayerMainHUD 생성 및 뷰포트 추가
    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UPlayerMainHUD>(this, HUDWidgetClass);

        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
            
            if (ABaseCharacter* CurChar = GetPawn<ABaseCharacter>())
            {
                // 체력 변경시 업데이트 함수가 호출되도록 델리게이트
                CurChar->OnHealthChangedDelegate.AddDynamic(HUDWidgetInstance, &UPlayerMainHUD::HPUpdate);

                // 플레이어 캐릭터 사망시 
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

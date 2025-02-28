// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

void UPlayerMainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerMainHUD::ShowPlayUI()
{
	PlayUI->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UPlayerMainHUD::HidePlayUI()
{
	PlayUI->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerMainHUD::HPUpdate(float NewHP, float MaxHP)
{
	if (HPBarFront)
	{
		FVector2D HPImageScale(NewHP / MaxHP, 1.f);
		HPBarFront->SetRenderScale(HPImageScale);
	}
	
	if (HPText)
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(NewHP))));
	}
}

void UPlayerMainHUD::AmmoVisibilityUpdate(bool IsVisibility)
{
	// 아직 미사용
	// 총을 장착했는지 확인 후 델리게이트를 사용해서 업데이트 되도록 구현해줘야함
	if (AmmoUI)
	{
		if (IsVisibility)
		{
			AmmoUI->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			AmmoUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPlayerMainHUD::AmmoUpdate(int NewAmmouCount, int MaxAmmouCount)
{
	// 아직 미사용
	// 총을 장착했는지 확인 후 델리게이트를 사용해서 업데이트 되도록 구현해줘야함
	if (MagAmmo)
	{
		MagAmmo->SetText(FText::FromString(FString::Printf(TEXT("%d"), NewAmmouCount)));
	}
	
	if (RemainAmmo)
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxAmmouCount)));
	}
}

void UPlayerMainHUD::ShowDeathUI()
{
	if (APlayerController* CurPlayerController = GetOwningPlayer())
	{
		CurPlayerController->SetInputMode(FInputModeUIOnly());
		CurPlayerController->SetShowMouseCursor(true);
	}

	DeathUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayerMainHUD::HideDeathUI()
{
	DeathUI->SetVisibility(ESlateVisibility::Hidden);
}

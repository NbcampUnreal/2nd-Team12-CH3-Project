// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMainHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "SPTPlayerCharacter.h"
#include "Weapons/WeaponBase.h"
#include "Data/WeaponDataStructs.h"
#include "Weapons/FirearmWeapon.h"

void UPlayerMainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

void UPlayerMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 가능하다면 틱 말고 델리게이트로 바인딩 하는게 성능에 유리함
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		ASPTPlayerCharacter* Player = Cast<ASPTPlayerCharacter>(PlayerController->GetPawn());
		if (Player)
		{
			// 장착중이 아니라면
			if (EFirearmType::EFT_MAX == Player->GetEquippedFirearmType())
			{
				AmmoVisibilityUpdate(false);
			}
			// 총을 장착중이라면
			else
			{
				AmmoVisibilityUpdate(true);

				if (AFirearmWeapon* FirearmWeapon = Cast<AFirearmWeapon>(Player->GetEquippedWeapon()))
				{
					// 전체 탄약 개수
					int32 MagazinCapacity = FirearmWeapon->GetMagazinCapacity();
					// 현재 탄약 개수
					int32 CurrentAmmo = FirearmWeapon->GetCurrentAmmo();

					AmmoUpdate(CurrentAmmo, MagazinCapacity);
				}
			}
		}
	}
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

	//DeathUI->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UPlayerMainHUD::HideDeathUI()
{
	//DeathUI->SetVisibility(ESlateVisibility::Hidden);
}

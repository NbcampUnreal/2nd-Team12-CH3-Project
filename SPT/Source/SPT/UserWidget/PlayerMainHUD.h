// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainHUD.generated.h"

class UTextBlock;
class UImage;
class UCanvasPanel;

UCLASS()
class SPT_API UPlayerMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HPUpdate(float NewHP, float MaxHP);

	UFUNCTION()
	void AmmoVisibilityUpdate(bool IsVisibility);

	UFUNCTION()
	void AmmoUpdate(int NewAmmouCount, int MaxAmmouCount);
	
private:
	// 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> HPText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UImage> HPBarFront;

	// 조준선
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UImage> AimingPoint;

	// 총알 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCanvasPanel> AmmoUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> MagAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UTextBlock> RemainAmmo;
};

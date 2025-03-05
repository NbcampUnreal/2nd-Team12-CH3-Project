// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainHUD.generated.h"

class UTextBlock;
class UImage;
class UCanvasPanel;
class UHitMarker;

UCLASS()
class SPT_API UPlayerMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

///////////////////////////////////////////////////////////////////////
// Play UI
public:
	UFUNCTION()
	void ShowPlayUI();

	UFUNCTION()
	void HidePlayUI();

	UFUNCTION()
	void HPUpdate(float NewHP, float MaxHP);

	UFUNCTION()
	void AmmoVisibilityUpdate(bool IsVisibility);

	UFUNCTION()
	void AmmoUpdate(int NewAmmouCount, int MaxAmmouCount);
	
private:
	// Play CanvasPanel
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCanvasPanel> PlayUI;
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
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Death UI

public:
	UFUNCTION()
	void ShowDeathUI();

	UFUNCTION()
	void HideDeathUI();

private:
	// Death CanvasPanel
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UCanvasPanel> DeathUI;

	//// Death CanvasPanel
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UUserWidget> DeathMenu;
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// Hit UI
public:
	void HitTarget();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = true, BindWidget))
	TObjectPtr<UHitMarker> HitMarker;
///////////////////////////////////////////////////////////////////////
};

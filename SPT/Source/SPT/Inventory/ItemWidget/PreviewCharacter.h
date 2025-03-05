// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreviewCharacter.generated.h"

class USpringArmComponent;
class USceneCaptureComponent2D;
class AWeaponBase;
class AFirearmWeapon;
class UItemDataObject;

UCLASS()
class SPT_API APreviewCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	APreviewCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* PreviewCharacterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* CaptureComp;

	// 프리뷰용 무기 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Preview")
	USkeletalMeshComponent* PreviewWeaponMesh;

	void RotatePreview(float DeltaYaw);

	void EquipWeapon(AFirearmWeapon* EquippedWeapon);
};

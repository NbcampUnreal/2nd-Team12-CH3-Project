// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreviewCharacter.generated.h"

class USpringArmComponent;
class USceneCaptureComponent2D;

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

	void RotatePreview(float DeltaYaw);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "SPTPlayerCharacter.h"

ASPTPlayerCharacter::ASPTPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASPTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASPTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


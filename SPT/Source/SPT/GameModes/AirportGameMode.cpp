// Fill out your copyright notice in the Description page of Project Settings.


#include "AirportGameMode.h"
#include "SPTPlayerCharacter.h"
#include "SPTPlayerController.h"

AAirportGameMode::AAirportGameMode()
{
	PlayerControllerClass = ASPTPlayerController::StaticClass();
	DefaultPawnClass = ASPTPlayerCharacter::StaticClass();
}

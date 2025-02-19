// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperimentGameMode.h"
#include "SPTPlayerCharacter.h"
#include "SPTPlayerController.h"

AExperimentGameMode::AExperimentGameMode()
{
	PlayerControllerClass = ASPTPlayerController::StaticClass();
	DefaultPawnClass = ASPTPlayerCharacter::StaticClass();
}

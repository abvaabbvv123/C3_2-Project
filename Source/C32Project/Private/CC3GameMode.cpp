// Fill out your copyright notice in the Description page of Project Settings.


#include "CC3GameMode.h"
#include "CC3GameState.h"
#include "CC3Character.h"
#include "CC3PlayerController.h"

ACC3GameMode::ACC3GameMode() {
	DefaultPawnClass = ACC3Character::StaticClass();
	PlayerControllerClass = ACC3PlayerController::StaticClass();
	GameStateClass = ACC3GameState::StaticClass();
}
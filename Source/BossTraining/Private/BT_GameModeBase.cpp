// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_GameModeBase.h"
#include "BT_Player.h"
#include "BT_PlayerController.h"

ABT_GameModeBase::ABT_GameModeBase() 
{
	DefaultPawnClass = ABT_Player::StaticClass();
	PlayerControllerClass = ABT_PlayerController::StaticClass();
}
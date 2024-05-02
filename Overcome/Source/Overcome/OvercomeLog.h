// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_SUBLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))
#define LOG_SUBREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))

#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), static_cast<int32>(GPlayInEditorID)) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER"))) 
#define LOG_SUBNETMODEINFO ((GetOwner()->GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), static_cast<int32>(GPlayInEditorID)) : ((GetOwner()->GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER"))) 

#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define OV_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define OV_SUBLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_SUBNETMODEINFO, LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogOVNetwork, Log, All);
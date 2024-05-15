// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVTeleportSkill.h"
#include "Character/OVCharacterPlayer.h"

UOVTeleportSkill::UOVTeleportSkill()
{
	PrimaryComponentTick.bCanEverTick = true;
	CoolTime = 10;
	TeleportOffset = 600.0f;
	TeleportCooltime = 3.0f;
}

void UOVTeleportSkill::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if (!Owner)
	// {
	// 	return;
	// }
	//
	// if (Owner->bIsActiveTeleportSkill) 
	// {
	// 	return;
	// }
	// FGameTime CurrentGameTime = GetWorld()->GetTime();
	// if (CurrentGameTime.GetWorldTimeSeconds() < ActivetedTimeStamp + CoolTime)
	// {
	//
	// 	float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolTime;
	// 	float CTTime= FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f);
	// 	//TeleCoolDown(CTTime);
	// }
	// else
	// {
	// 	float ElapsedTime =( CurrentGameTime.GetWorldTimeSeconds() - ActivetedTimeStamp)/CoolTime;
	// 	float CTTime=  FMath::Clamp(1.0f-ElapsedTime, 0.0f, 1.0f)/CoolTime;
	// 	//TeleCoolDown(CTTime);
	// 	Owner->bIsActiveTeleportSkill = true;
	// }
}

void UOVTeleportSkill::SkillAction()
{
	Super::SkillAction();
	FVector TargetLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * TeleportOffset;
	GetOwner()->TeleportTo(TargetLocation, GetOwner()->GetActorRotation(), false, true);
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveTeleportSkill = true;
			UE_LOG(LogTemp, Warning, TEXT("TeleportSkill"));
		}
	), TeleportCooltime, false, -1.0f);
}

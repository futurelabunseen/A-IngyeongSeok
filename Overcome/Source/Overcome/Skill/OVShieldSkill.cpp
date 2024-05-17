// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/OVShieldSkill.h"

#include "Character/OVCharacterPlayer.h"

UOVShieldSkill::UOVShieldSkill()
{
	ShieldCooltime = 5.f;
}

void UOVShieldSkill::SkillAction()
{
	UE_LOG(LogSkillCharacter, Log, TEXT("StartShieldSkill"));
	Super::SkillAction();
	
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]
		{
			Owner->bIsActiveShieldSkill = true;
			UE_LOG(LogSkillCharacter, Log, TEXT("EndShieldSkill"));
		}
	), ShieldCooltime, false, -1.0f);
}

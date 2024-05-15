// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHUDWidget.h"

#include "OVTargetWidget.h"
#include "Interface/OVCharacterHUDInterface.h"

UOVHUDWidget::UOVHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{       
}

void UOVHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TargetWidget = Cast<UOVTargetWidget>(GetWidgetFromName(TEXT("WBP_TargetWidget")));

	IOVCharacterHUDInterface* CharacterWidget = Cast<IOVCharacterHUDInterface>(GetOwningPlayerPawn());
	if(CharacterWidget)
	{
		CharacterWidget->SetupHUDWidget(this);
	}
}

void UOVHUDWidget::UpdateTarget(bool bIsShowUI)
{
	TargetWidget->UpdateTargetUI(bIsShowUI);
}

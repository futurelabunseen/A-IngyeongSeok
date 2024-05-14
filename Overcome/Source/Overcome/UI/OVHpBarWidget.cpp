// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OVHpBarWidget.h"

#include "CookedMetaData.h"
#include "Components/ProgressBar.h"

UOVHpBarWidget::UOVHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UOVHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
}

void UOVHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	 ensure(MaxHp > 0.0f);

	if(HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp/MaxHp);
	}
}

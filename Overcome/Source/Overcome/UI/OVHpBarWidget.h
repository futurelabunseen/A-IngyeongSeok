// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OVHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOVHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual  void NativeConstruct() override;
	
public:
	FORCEINLINE void SetMaxHp(float NewMAxHp){MaxHp = NewMAxHp;}
	void UpdateHpBar(float NewCurrentHp);
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};

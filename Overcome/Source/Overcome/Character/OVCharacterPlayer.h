// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/OVCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "OVCharacterPlayer.generated.h"

/**
 * 
 */

class AOVGun;

UCLASS()
class OVERCOME_API AOVCharacterPlayer : public AOVCharacterBase
{
	GENERATED_BODY()

public:
	AOVCharacterPlayer();

protected:
	virtual void BeginPlay() override;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Timeline Section
	FOnTimelineFloat SmoothCrouchInterpFunction; // (1)
	FOnTimelineEvent SmoothCrouchTimelineFinish; // (2)
	UFUNCTION()
	void SmoothCrouchInterpReturn(float Value); // (3)
	UFUNCTION()
	void SmoothCrouchOnFinish(); // (4)
	UPROPERTY()
	UTimelineComponent* SmoothCrouchingCurveTimeline; // (5)
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* SmoothCrouchingCurveFloat; // (6)


	// Character Control Section
protected:
	void ChangeCharacterControl(); //V키 눌러서 컨트롤러 바꿨을 때 구현하는 함수
	void SetCharacterControl(ECharacterControlType NewCharacterControlType); //변경됐을 때 컨트롤 설정 바꾸는 함수
	virtual void SetCharacterControlData(const class UOVCharacterControlData* CharacterControlData) override;

	//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	//Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookActionX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookActionY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimAction;//마우스 오른쪽

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WheelAction;

	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLookX(const FInputActionValue& Value);
	void ShoulderLookY(const FInputActionValue& Value);

	void QuaterMove(const FInputActionValue& Value);

	void Aiming(const FInputActionValue& Value);
	void StopAiming(const FInputActionValue& Value);

	void Jumping(const FInputActionValue& Value);

	void ChangeWeapon(const FInputActionValue& Value);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsAiming : 1; //조준 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	uint8 bIsGun : 1;

public:
	const uint8 GetIsAiming() { return bIsAiming; };
	ECharacterControlType CurrentCharacterControlType;

protected:
	//Turn In Place Section
	UFUNCTION(BlueprintCallable, Category = "Turn")
	void PlayTurn(class UAnimMontage* MontagetoPlay, float PlayRate, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TurnRight90();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TurnLeft90();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TurnRight180();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TurnLeft180();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void ClearTurnInPlace(float Force);

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void ClearMotion();

	UFUNCTION(BlueprintCallable, Category = "Turn")
	void TurnInPlace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	uint8 bIsTurning : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	TObjectPtr<class UAnimMontage> TurnRight_90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	TObjectPtr<class UAnimMontage> TurnLeft_90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	TObjectPtr<class UAnimMontage> TurnRight_180;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	TObjectPtr<class UAnimMontage> TurnLeft_180;


	//Gun
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AOVGun> GunClass;
protected:
	void Shoot();
	void StopShoot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	uint8 bIsShooting : 1;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	TObjectPtr<class UAnimMontage> Shooting_Gun;

	UPROPERTY()
	AOVGun* Gun;


};
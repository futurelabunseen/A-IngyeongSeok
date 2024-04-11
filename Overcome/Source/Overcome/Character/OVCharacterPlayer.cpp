// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/OVCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Gun/OVGun.h"
#include "OVCharacterControlData.h"


AOVCharacterPlayer::AOVCharacterPlayer()
{
	//Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SocketOffset = FVector(0.0, 80.0, 50.0);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Jump.IA_OV_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ChangeControl.IA_OV_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderMove.IA_OV_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookXRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderLookX.IA_OV_ShoulderLookX'"));
	if (nullptr != InputActionShoulderLookXRef.Object)
	{
		ShoulderLookActionX = InputActionShoulderLookXRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookYRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ShoulderLookY.IA_OV_ShoulderLookY'"));
	if (nullptr != InputActionShoulderLookYRef.Object)
	{
		ShoulderLookActionY = InputActionShoulderLookYRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_QuaterMove.IA_OV_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_MouseRight.IA_OV_MouseRight'"));
	if (nullptr != AimActionRef.Object)
	{
		AimAction = AimActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShootActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_Shoot.IA_OV_Shoot'"));
	if (nullptr != ShootActionRef.Object)
	{
		ShootAction = ShootActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WheelActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_OV_ChangeWeapon.IA_OV_ChangeWeapon'"));
	if (nullptr != WheelActionRef.Object)
	{
		WheelAction = WheelActionRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;
	bIsAiming = false;

	//Timeline
	SmoothCrouchingCurveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineFront"));
	SmoothCrouchInterpFunction.BindUFunction(this, FName("SmoothCrouchInterpReturn"));
	SmoothCrouchTimelineFinish.BindUFunction(this, FName("SmoothCrouchOnFinish"));

	//Gun
	Gun = CreateDefaultSubobject<AOVGun>(TEXT("Gun"));
}

void AOVCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	Gun = GetWorld()->SpawnActor<AOVGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
	//Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
	Gun->SetOwner(this);
	Gun->SetActorEnableCollision(false);

	if (SmoothCrouchingCurveFloat)
	{
		SmoothCrouchingCurveTimeline->AddInterpFloat(SmoothCrouchingCurveFloat, SmoothCrouchInterpFunction);
		SmoothCrouchingCurveTimeline->SetTimelineFinishedFunc(SmoothCrouchTimelineFinish);
		SmoothCrouchingCurveTimeline->SetLooping(false);
	}
	SetCharacterControl(CurrentCharacterControlType);
}

void AOVCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Jumping);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookActionX, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShoulderLookX);
	EnhancedInputComponent->BindAction(ShoulderLookActionY, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ShoulderLookY);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::QuaterMove);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Aiming);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AOVCharacterPlayer::StopAiming);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::Shoot);
	EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AOVCharacterPlayer::StopShoot);
	EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &AOVCharacterPlayer::ChangeWeapon);

}

void AOVCharacterPlayer::SmoothCrouchInterpReturn(float Value)
{
	CameraBoom->TargetArmLength = (FMath::Lerp(250, 150, Value));
}

void AOVCharacterPlayer::SmoothCrouchOnFinish()
{
}


void AOVCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AOVCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UOVCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType]; //컨트롤 데이터 가지고 온다
	check(NewCharacterControl); //반드시 있는지 확인

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); //컨트롤러 가지고 오기 CastChecked로 확인

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings(); //매핑 초기화
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	CurrentCharacterControlType = NewCharacterControlType;
}

void AOVCharacterPlayer::SetCharacterControlData(const UOVCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}


void AOVCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);

	ClearTurnInPlace(MovementVector.X);
	ClearTurnInPlace(MovementVector.Y);
}


void AOVCharacterPlayer::ShoulderLookX(const FInputActionValue& Value)
{
	float LookAxisVector = Value.Get<float>();

	AddControllerYawInput(LookAxisVector);
	//UE_LOG(LogTemp, Log, TEXT("LookX"));

	TurnInPlace();
}

void AOVCharacterPlayer::ShoulderLookY(const FInputActionValue& Value)
{
	float LookAxisVector = Value.Get<float>();

	AddControllerPitchInput(LookAxisVector);
}

void AOVCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}
	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void AOVCharacterPlayer::Aiming(const FInputActionValue& Value)
{
	bIsAiming = true;
	SmoothCrouchingCurveTimeline->Play();
	
}

void AOVCharacterPlayer::StopAiming(const FInputActionValue& Value)
{
	bIsAiming = false;
	SmoothCrouchingCurveTimeline->Reverse();
}


void AOVCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!bIsAiming)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
}

void AOVCharacterPlayer::ChangeWeapon(const FInputActionValue& Value)
{
	float TEST = Value.Get<float>();
	UE_LOG(LogTemp, Log, TEXT("Change Weapon: %f"), TEST);
	int GunSocket = TEST;
	if(GunSocket > 0)
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Rifle_Socket"));
	else
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Back_Socket"));
}

void AOVCharacterPlayer::PlayTurn(class UAnimMontage* MontagetoPlay, float PlayRate, float Duration)
{
	if (!bIsTurning)
	{
		bIsTurning = true;
		PlayAnimMontage(MontagetoPlay, PlayRate);
		// Declare the FTimerHandle within the function
		FTimerHandle TimerHandle;

		// Set up the timer to call the ResetTurning function after 0.2 seconds
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				this->bIsTurning = false;
			}, Duration, false);

		bIsTurning = false;
	}
}

void AOVCharacterPlayer::TurnRight90()
{
	PlayTurn(TurnRight_90, 1.5f, 0.5f);
}

void AOVCharacterPlayer::TurnLeft90()
{
	PlayTurn(TurnLeft_90, 1.5f, 0.5f);
}

void AOVCharacterPlayer::TurnRight180()
{
	PlayTurn(TurnRight_180, 1.7f, 0.6f);
}

void AOVCharacterPlayer::TurnLeft180()
{
	PlayTurn(TurnLeft_180, 1.7f, 0.6f);
}

void AOVCharacterPlayer::ClearTurnInPlace(float Force)
{
	if (Force != 0.0f)
	{
		ClearMotion();
	}
}

void AOVCharacterPlayer::ClearMotion()
{
	if (IsPlayingRootMotion())
	{
		StopAnimMontage(GetCurrentMontage());
	}

}

void AOVCharacterPlayer::TurnInPlace()
{
	float VelocityXY = GetCharacterMovement()->Velocity.Size2D();
	if (!(GetCharacterMovement()->IsFalling()) && !(VelocityXY > 0.0f))
	{
		FRotator DeltaRotation = GetActorRotation() - GetBaseAimRotation();
		DeltaRotation.Normalize();
		float DeltaYaw = DeltaRotation.Yaw * -1.0f;

		if ((DeltaYaw > 45.f) || (DeltaYaw < -45.f))
		{
			if (DeltaYaw > 135.f)
				TurnRight180();
			else if (DeltaYaw < -135.f)
				TurnLeft180();
			else if(DeltaYaw > 45.f)
				TurnRight90();
			else if (DeltaYaw < -45.f)
				TurnLeft90();
		}
	}
}

void AOVCharacterPlayer::Shoot()
{
	if (!bIsShooting)
	{
		bIsShooting = true;
		Gun->PullTrigger();
		PlayAnimMontage(Shooting_Gun, 0.5);

		FTimerHandle TimerHandle;

		// Set up the timer to call the ResetTurning function after 0.2 seconds
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
			{
				bIsShooting = false;
			}, 0.5, false);
	}
}

void AOVCharacterPlayer::StopShoot()
{

}


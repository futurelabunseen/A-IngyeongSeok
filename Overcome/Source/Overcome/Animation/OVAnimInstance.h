// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OVAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OVERCOME_API UOVAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UOVAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override; //�ִԱ׷��� ���۽� �۵�

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //�����Ӹ��� ��� ������Ʈ 


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner; //�ִ� �ν��Ͻ��� �����ϰ� �ִ� ������ ��� ���� 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // ����� ��Ȯ�� bool Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould; //�����̴��� 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //���������ִ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //�����ϰ��ִ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould; //���������� 
};

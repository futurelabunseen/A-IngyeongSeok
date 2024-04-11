// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun/OVGun.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AOVGun::AOVGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	
	Mesh->SetupAttachment(Root);
}

void AOVGun::PullTrigger()
{
	//UE_LOG(LogTemp, Log, TEXT("You've been shot!"));
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if(OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange; 

	//DrawDebugPoint(GetWorld(), Location, 20, FColor::Red, true);
	//DrawDebugCamera(GetWorld(), Location, Rotation, 90, 2, FColor::Red, true);

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		FVector ShotDirection = -Rotation.Vector();
		DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent{ Damage, Hit, ShotDirection, nullptr };
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}


// Called when the game starts or when spawned
void AOVGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOVGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


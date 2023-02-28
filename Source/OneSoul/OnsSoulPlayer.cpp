// Fill out your copyright notice in the Description page of Project Settings.


#include "OnsSoulPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AOnsSoulPlayer::AOnsSoulPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
   
   SpringArm= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
   SpringArm->SetupAttachment(GetRootComponent());
   SpringArm->TargetArmLength =300.f;

   Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
   Camera->SetupAttachment(SpringArm);

   GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f),FRotator(0.f, 90.f, 0.f));
   GetMesh()->SetRelativeScale3D(FVector(0.1f));
}

// Called when the game starts or when spawned
void AOnsSoulPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOnsSoulPlayer::MoveForward(float Value)
{
 FVector Forward = GetActorForwardVector();
 AddMovementInput(Forward,Value);
}

// Called every frame
void AOnsSoulPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOnsSoulPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOnsSoulPlayer::MoveForward);

}


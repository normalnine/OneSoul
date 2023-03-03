// Fill out your copyright notice in the Description page of Project Settings.


#include "OnsSoulPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"

AOnsSoulPlayer::AOnsSoulPlayer() :
    BaseTurnRate(45.f),
	BaseLookUpRate(45.f)

{
 
   PrimaryActorTick.bCanEverTick = true;
   
   bUseControllerRotationPitch=false;
   bUseControllerRotationRoll=false;
   bUseControllerRotationYaw=false;

   GetCharacterMovement()-> bOrientRotationToMovement = true;
   GetCharacterMovement()-> RotationRate = FRotator(0.f, 400.f, 0.f);
   GetCharacterMovement()-> JumpZVelocity = 600.f;
   GetCharacterMovement()-> AirControl = 0.2f;

   SpringArm= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
   SpringArm->SetupAttachment(GetRootComponent());
   SpringArm->TargetArmLength =300.f;
   SpringArm->bUsePawnControlRotation=true;

   Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
   Camera->SetupAttachment(SpringArm);
   Camera->bUsePawnControlRotation=false;

   Wing=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wing"));
   Wing->SetupAttachment(GetRootComponent());

   GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f),FRotator(0.f, 90.f, 0.f));
   GetMesh()->SetRelativeScale3D(FVector(0.1f));

   SetPlayerMaxSpeed(WalkSpeed);

   TargetingDistance = 900.f;
   TargetingRadius = 100.f;
   TargetRotationInterpSpeed = 9.f;

   bIsTargeting = false;
   SetParachute = false;

}

void AOnsSoulPlayer::BeginPlay()
{
	Super::BeginPlay();

	Controller = this->GetController();

	switch (RotationMode)
	{
	case ERotationMode::OrienttoCamera:
		this->bUseControllerRotationYaw = false;
		this->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		this->GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	case ERotationMode::OrienttoMovement:
		this->bUseControllerRotationYaw = false;
		this->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		this->GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	}
	
}

void AOnsSoulPlayer::MoveForward(float Value)
{
	if((Controller != nullptr) && (Value != 0.0f))
	{
     const FRotator ControlRotation = GetControlRotation();
     const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);

     const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
     AddMovementInput(Direction,Value);
	}

}

void AOnsSoulPlayer::MoveRight(float Value)
{
	
	if ((Controller != nullptr) && (Value != 0.0f))
	{ 
	
     const FRotator ControlRotation = GetControlRotation();
     const FRotator YawRotatin(0.f,ControlRotation.Yaw,0.f);

     const FVector Direction =FRotationMatrix(YawRotatin).GetUnitAxis(EAxis::Y);
     AddMovementInput(Direction,Value);
	
	}
}

void AOnsSoulPlayer::Turn(float Value)
{
 AddControllerYawInput(Value);
}

void AOnsSoulPlayer::LookUp(float Value)
{
 AddControllerPitchInput(Value);
}


void AOnsSoulPlayer::TurnAtRate(float Rate)
{
 AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
}

void AOnsSoulPlayer::LookUpRate(float Rate)
{
 AddControllerPitchInput(Rate*BaseLookUpRate*GetWorld()->GetDeltaSeconds());
}

void AOnsSoulPlayer::ToggleLockOnInput()
{
 ToggleLockOn();
}

void AOnsSoulPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTargeting)
	{
		UpdateTargetingControlRotation();
	}

	//Wings();

}

void AOnsSoulPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AOnsSoulPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AOnsSoulPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn",this,&AOnsSoulPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp",this,&AOnsSoulPlayer::LookUp);
	PlayerInputComponent->BindAxis("TurnRate",this,&AOnsSoulPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate",this,&AOnsSoulPlayer::LookUpRate);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AOnsSoulPlayer::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&AOnsSoulPlayer::StopJumping);
	PlayerInputComponent->BindAction("Sprint",IE_Pressed,this,&AOnsSoulPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint",IE_Released,this,&AOnsSoulPlayer::StopSprint);
	PlayerInputComponent->BindAction("ToggleLockOn",IE_Pressed,this,&AOnsSoulPlayer::ToggleLockOnInput);
}

void AOnsSoulPlayer::StartSprint()
{
	if (CurrentStamina > MinStamina)
	{
		SetPlayerMaxSpeed(SprintSpeed);
		UKismetSystemLibrary::K2_SetTimer(this,"SprintTimer", 0.1f, true);
	}
}

void AOnsSoulPlayer::SetPlayerMaxSpeed(float MaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}
void AOnsSoulPlayer::SprintTimer()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - 1.f, MinStamina, MaxStamina);
	if (CurrentStamina <= MinStamina)
	{
		StopSprint();
	}
}

void AOnsSoulPlayer::StopSprint()
{
	UKismetSystemLibrary::K2_ClearTimer(this,"SprintTimer");
	SetPlayerMaxSpeed(WalkSpeed);
}

void AOnsSoulPlayer::EnableLockOn()
{
	FVector Start = this->GetActorLocation();
	FVector End = Camera->GetForwardVector() * TargetingDistance+Start;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	TargetObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;

	bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		Start,
		End,
		TargetingRadius,
		TargetObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true);

	auto HitActor = UKismetSystemLibrary::IsValid(HitResult.GetActor());

	if (Result == true && HitActor)
	{
		TargetActor = HitResult.GetActor();
		bIsTargeting = true;
		RotationMode = ERotationMode::OrienttoCamera;
	}

}

void AOnsSoulPlayer::DisableLockOn()
{
	bIsTargeting = false;
	RotationMode = ERotationMode::OrienttoMovement;
}

void AOnsSoulPlayer::ToggleLockOn()
{
	if (!bIsTargeting)
	{
		EnableLockOn();
	}
	else
	{
		DisableLockOn();
	}
}

void AOnsSoulPlayer::UpdateTargetingControlRotation()
{
	auto OChar = UKismetSystemLibrary::IsValid(this);
	auto Target = UKismetSystemLibrary::IsValid(TargetActor);

	if (OChar && Target)
	{
		FVector Start = this->GetActorLocation();
		FVector TargetR = TargetActor->GetActorLocation() - FVector(0.f,0.f,100.f);
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Start, TargetR);
		FRotator CurrentRot = Controller->GetControlRotation();

		FRotator YZ = UKismetMathLibrary::RInterpTo(
			CurrentRot,
			TargetRot,
			UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			TargetRotationInterpSpeed);


		FRotator NewRotation = UKismetMathLibrary::MakeRotator(CurrentRot.Roll, YZ.Pitch, YZ.Yaw);

		Controller->SetControlRotation(NewRotation);
	}
}
void AOnsSoulPlayer:: Wings()
{
	if (SetParachute)
	{
	  FVector Velo = UKismetMathLibrary::MakeVector(GetVelocity().X,GetVelocity().Y,-300.f);

	  GetCharacterMovement()->AirControl=200.f;
	  GetCharacterMovement()->GravityScale=0.05f;
	  GetCharacterMovement()->Velocity = Velo;
	}
	else
	{
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 1.f;
	if (GetCharacterMovement()->IsFalling())
	{
		SetParachute=true;
		Wing->SetVisibility(true);
	}
	
	Wing->SetVisibility(false);

	}
}
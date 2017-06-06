// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPawn.h"
#include "HowTo_PlayerInput.h"


// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create dummy root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Create camera
	UCameraComponent* ourCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	m_OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	//Attach our camera
	ourCamera->SetupAttachment(RootComponent);
	ourCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	ourCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	m_OurVisibleComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float currentScale = m_OurVisibleComponent->GetComponentScale().X;
	if (m_bGrowing)
	{
		currentScale += DeltaTime;
	}
	else
	{
		currentScale -= DeltaTime*0.5f;
	}

	currentScale = FMath::Clamp(currentScale, 1.0f, 2.0f);
	m_OurVisibleComponent->SetWorldScale3D(FVector(currentScale));

	if (!m_CurrentVelocity.IsZero())
	{
		FVector newLoc = GetActorLocation() + (m_CurrentVelocity*DeltaTime);
		SetActorLocation(newLoc);
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Respond to GROW
	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	//Respond to axis
	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);


}

void AMyPawn::Move_XAxis(float AxisValue)
{
	//Move at 100 units per second
	m_CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f)*100.0f;
}

void AMyPawn::Move_YAxis(float AxisValue)
{
	//Move at 100 units per second
	m_CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f)*100.0f;
}

void AMyPawn::StartGrowing()
{
	m_bGrowing = true;
}

void AMyPawn::StopGrowing()
{
	m_bGrowing = false;
}


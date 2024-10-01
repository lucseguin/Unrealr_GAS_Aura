// Copyright Luc Seguin


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Player/AuraPlayerState.h"
AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//GetPlayerState<AAuraPlayerState>()
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotator(0.f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursoHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursoHit);
	if(CursoHit.bBlockingHit)
	{
		LastActor = CurrentActor;
		CurrentActor = CursoHit.GetActor();

		/**
		 * Line trace from cursor, there are several possible scenarios
		 * 1. LastActor is null && CurrentActor is null
		 *	  - Do Nothing
		 * 2. LastActor is null && CurrentActor is valid
		 *    - Highlight this actor if itès the first frame it got selected
		 * 3. LastActor is valid && CurrentActor is null
		 *    - UnHilight this actor, we are not hovering it anymore
		 * 4. LastActor and CurrentActor are valid, but they are different
		 *    - UnHilight LastActor and Highlight CurrentActor
		 * 5. LastActor and CurrentActor are valid, and are the same
		 *    - do nothing
		 */

		if(LastActor == nullptr)
		{
			if(CurrentActor != nullptr)
			{
				// case 2
				CurrentActor->HighlightActor();
			} //else case 1, do nothing
		}
		else //LastActor is Valid
		{
			if(CurrentActor == nullptr)
			{
				//case 3
				LastActor->UnHighlightActor();
			}
			else // both actors are valid
			{
				if(LastActor != CurrentActor)
				{
					//case 4
					LastActor->UnHighlightActor();
					CurrentActor->HighlightActor();
				} //else case 5 do nothing
			}
		}
	}
}

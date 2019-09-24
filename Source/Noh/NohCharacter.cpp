#include "NohCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "AnimNotifyState_Pivot.h"
#include "NohHUD.h"
#include "Katana.h"

//Default constructor//
ANohCharacter::ANohCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 90.0f);

	//Main Player States//
	movementmode = E_MOVEMENTMODE::MM_GROUNDED;
	prevmovementmode = E_MOVEMENTMODE::MM_NONE;
	gaitmode = E_GAIT::GT_RUNNING;
	stancemode = E_STANCEMODE::SM_STANDING;
	rotationmode = E_ROTATIONMODE::RM_LOOKINGDIRECTION;
	//rotationmode = E_ROTATIONMODE::RM_VELOCITYDIRECTION;
	movementdirection = E_MOVEMENTDIRECTION::MD_FORWARDS;
	activelocostate = E_ACTIVELOCOSTATE::ALS_NOTMOVING;
	idleentrystate = E_IDLEENTRYSTATE::N_IDLE;
	cardinaldirection = E_CARDINALDIRECTION::CD_NORTH;
	footsteptype = E_FOOTSTEPTYPE::FST_STEP;
	activeweapon = E_ACTIVEWEAPON::AW_NONE;

	//Player Aim State//
	b_aiming = false;

	//Sub Character States//
	b_shouldsprint = false;

	//Player Movement State//
	b_ismoving = false;
	b_hasmovementinput = false;

	//Player Turn In Place State//
	b_turninginplace = false;
	b_shouldturninplace = false;
	b_turningright = false;
	turninplacedelaycount = 0.0f;

	//Default Character Movement Settings//
	GetCharacterMovement()->CrouchedHalfHeight = 60.0f;
	GetCharacterMovement()->MaxStepHeight = 45.0f;
	GetCharacterMovement()->SetWalkableFloorAngle(50.0f);
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = walkspeed;

	//Player Movement Speed//
	speed = 0.0f;
	previousspeed = 0.0f;
	walkspeed = 165.0f;
	runspeed = 375.0f;
	sprintspeed = 600.0f;
	crouchspeed = 150.0f;

	//Player Current Velocity//
	nohcharactervelocity = FVector{ 0.0f };

	//Player Movement Input and Velocity Difference//
	movementinput_velocitydifference = 0.0f;

	//Player Character Rotation and Target Rotation Difference//
	targetcharacter_rotationdifference = 0.0f;

	//Character Movement acceleration
	walkacceleration = 800.0f;
	runacceleration = 1000.0f;

	//Character Movement deceleration
	walkdeceleration = 800.0f;
	rundeceleration = 800.0f;

	//Character Movement friction
	walkgroundfriction = 8.0f;
	rungroundfriction = 6.0f;

	//Character Rotation//
	rotationoffset = 0.0f;
	rotationratemultiplier = 1.0f;
	rotation_lastvelocity = GetActorRotation();
	rotation_previousvelocity = FRotator{ 0.0f };
	rotation_looking = GetActorRotation();
	rotation_lastmovementinput = GetActorRotation();
	rotation_target = GetActorRotation();
	rotation_character = GetActorRotation();
	rotation_jump = FRotator{ 0.0f };
	direction = 0.0f;

	//Axis Values//
	forwardaxisvalue = 0.0f;
	rightaxisvalue = 0.0f;

	//Do Once//
	b_dooncesprint = false;
	b_dooncerun = false;

	//Player Camera Sensitivity//
	verticalcontrollersensitivity = 150.0f;
	horizontalcontrollersensitivity = 150.0f;

	//Camera boom (Spring that pulls the player camera in towards the character if there is a collision)//
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 325.0f;										// The camera follows at this distance behind the character
	CameraBoom->SocketOffset = { 0.0f, 0.0f, 45.0f };
	CameraBoom->bEnableCameraRotationLag = false;
	CameraBoom->CameraLagSpeed = 10.0f;

	//Follow Camera (Main player camera)//
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bConstrainAspectRatio = false;
	FollowCamera->bUsePawnControlRotation = false;

	//Camera Timeline//
	cameralerptimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimeline"));

	//Yaw Angle Difference Between Looking Rotation And Character Rotation//
	aimyawdelta = 0.0f;

	//Current Camera Turn Rate//
	aimyawrate = 0.0f;

	//Aim right or left shoulder//
	b_rightshoulderaim = true;

	//Current Aim Camera Settings//
	currentcamera_armlength = 0.0f;
	currentcamera_lagspeed = 0.0f;
	currentcamera_socketoffset = FVector{0.0f};

	//Target Aim Camera Settings//
	targetcamera_armlength = 0.0f;
	targetcamera_lagspeed = 0.0f;
	targetcamera_socketoffset = FVector{0.0f};

	targetcamera_velocitydirection_walk_armlength = 300.0f;
	targetcamera_velocitydirection_walk_lagspeed = 10.0f;
	targetcamera_velocitydirection_walk_socketoffset = FVector{ 0.0f, 0.0f, 45.0f };
	targetcamera_velocitydirection_run_armlength = 325.0f;
	targetcamera_velocitydirection_run_lagspeed = 8.0f;
	targetcamera_velocitydirection_run_socketoffset = FVector{ 0.0f, 0.0f, 45.0f };
	targetcamera_velocitydirection_sprint_armlength = 400.0f;
	targetcamera_velocitydirection_sprint_lagspeed = 6.0f;
	targetcamera_velocitydirection_sprint_socketoffset = FVector{ 0.0f, 0.0f, 30.0f };
	targetcamera_velocitydirection_crouching_armlength = 275.0f;
	targetcamera_velocitydirection_crouching_lagspeed = 10.0f;
	targetcamera_velocitydirection_crouching_socketoffset = FVector{ 0.0f, 20.0f, 45.0f };

	targetcamera_lookdirection_walk_armlength = 275.0f;
	targetcamera_lookdirection_walk_lagspeed = 15.0f;
	targetcamera_lookdirection_walk_socketoffset = FVector{ 0.0f, 70.0f, 60.0f };
	targetcamera_lookdirection_run_armlength = 300.0f;
	targetcamera_lookdirection_run_lagspeed = 15.0f;
	targetcamera_lookdirection_run_socketoffset = FVector{ 0.0f, 60.0f, 60.0f };
	targetcamera_lookdirection_sprint_armlength = 325.0f;
	targetcamera_lookdirection_sprint_lagspeed = 15.0f;
	targetcamera_lookdirection_sprint_socketoffset = FVector{ 0.0f, 50.0f, 50.0f };
	targetcamera_lookdirection_crouching_armlength = 250.0f;
	targetcamera_lookdirection_crouching_lagspeed = 15.0f;
	targetcamera_lookdirection_crouching_socketoffset = FVector{ 0.0f, 60.0f, 45.0f };

	targetcamera_aiming_armlength = 200.0f;
	targetcamera_aiming_lagspeed = 20.0f;
	targetcamera_aiming_socketoffset = FVector{ 0.0f, 70.0f, 45.0f };

	targetcamera_ragdoll_armlength = 350.0f;
	targetcamera_ragdoll_lagspeed = 50.0f;
	targetcamera_ragdoll_socketoffset = FVector{ 0.0f, 0.0f, -20.0f };

	//Camera Lerp Curve Assets//
	static ConstructorHelpers::FObjectFinder<UCurveFloat> cameracurve1(TEXT("/Game/MainCharacter/Curves/Camera/FC_cameralerp1"));
	if (cameracurve1.Object) {
		curve_cameralerp1 = cameracurve1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> cameracurve2(TEXT("/Game/MainCharacter/Curves/Camera/FC_cameralerp2"));
	if (cameracurve2.Object) {
		curve_cameralerp2 = cameracurve2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> cameracurve3(TEXT("/Game/MainCharacter/Curves/Camera/FC_cameralerp3"));
	if (cameracurve3.Object) {
		curve_cameralerp3 = cameracurve3.Object;
	}

	//Character Ragdoll//
	ragdoll_velocity = FVector{ 0.0f };
	b_ragdollonground = false;

	//Player Animation Montages//
	activeturninplacemontage = nullptr;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_getupfromfrontobj(TEXT("/Game/MainCharacter/Animations/GettingUp/C_GetUpFromFront_Montage"));
	if (animmontage_getupfromfrontobj.Object) {
		getupfromfront = animmontage_getupfromfrontobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_getupfrombackobj(TEXT("/Game/MainCharacter/Animations/GettingUp/C_GetUpFromBack_Montage"));
	if (animmontage_getupfrombackobj.Object) {
		getupfromback = animmontage_getupfrombackobj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_lfturnl90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/LF_Turn_L90_Montage"));
	if (animmontage_lfturnl90obj.Object) {
		lfturnl90 = animmontage_lfturnl90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_lfturnr90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/LF_Turn_R90_Montage"));
	if (animmontage_lfturnr90obj.Object) {
		lfturnr90 = animmontage_lfturnr90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_cturnl90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/C_Turn_L90_Montage"));
	if (animmontage_cturnl90obj.Object) {
		cturnl90 = animmontage_cturnl90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_cturnr90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/C_Turn_R90_Montage"));
	if (animmontage_cturnr90obj.Object) {
		cturnr90 = animmontage_cturnr90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nturnl90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/N_Turn_L90_Montage"));
	if (animmontage_nturnl90obj.Object) {
		nturn_l_90 = animmontage_nturnl90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nturnr90obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/N_Turn_R90_Montage"));
	if (animmontage_nturnr90obj.Object) {
		nturn_r_90 = animmontage_nturnr90obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nturnl180obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/N_Turn_L180_Montage"));
	if (animmontage_nturnl180obj.Object) {
		nturn_l_180 = animmontage_nturnl180obj.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nturnr180obj(TEXT("/Game/MainCharacter/Animations/TurnInPlace/N_Turn_R180_Montage"));
	if (animmontage_nturnr180obj.Object) {
		nturn_r_180 = animmontage_nturnr180obj.Object;
	}

	//Character Anim Blend Curves//
	static ConstructorHelpers::FObjectFinder<UCurveFloat> alphaflailcurve(TEXT("/Game/MainCharacter/Curves/AnimBlend/FC_flailalpha"));
	if (alphaflailcurve.Object) {
		curve_flailalpha = alphaflailcurve.Object;
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> landpredictioncurve(TEXT("/Game/MainCharacter/Curves/AnimBlend/FC_landpredictionalpha"));
	if (landpredictioncurve.Object) {
		curve_landpredictionalpha = landpredictioncurve.Object;
	}

	//Player Head Aim Offset//
	aimoffset = { 0.0f, 0.0f };

	//Player Gait Value//
	gaitvalue = 0.0f;

	//Player Locomotion Blendspace Play Rates//
	n_playrate = 0.0f;
	c_playrate = 0.0f;

	//Player Locomotion Blendspace Start Position//
	startposition = 0.0f;

	//Player Lean Blendspace Direction//
	leangrounded = FVector2D{ 0.0f, 0.0f };

	//Character Falling Flail Blend Alpha//
	flailblendalpha = 0.0f;

	//Character Ragdoll Flail Rate//
	ragdoll_flailrate = 0.0f;

	//Character Ground Landing Prediction Blend Alpha//
	landpredictionalpha = 0.0f;

	//Character Air Leaning Value//
	leaninair = 0.0f;

	//Character Feet Position//
	feetposition = { 0.0f, 0.0f };

	//Character Feet IK State//
	b_shouldfootik = true;
	ikfeettracelengthabovefeet = 50.0f;
	ikfeettracelengthbelowfeet = 75.0f;
	ikfeetminstandingclamplimit = FVector{ -10.0f, -30.0f, -30.0f };
	ikfeetmincrouchingclamplimit = FVector{ -10.0f, -30.0f, -15.0f };
	ikfeetmaxstandingclamplimit = FVector{ 10.0f, 30.0f, 20.0f };
	ikfeetmaxcrouchingclamplimit = FVector{ 10.0f, 30.0f, 10.0f };
	ikleftfootoffset = FVector{ 0.0f };
	ikrightfootoffset = FVector{ 0.0f };
	ikpelvisoffset = 0.0f;

	//Character Sheath State//
	b_issheathed = true;
	b_issheathing = false;

	//Enemy Hit Flag//
	b_enemyhit = false;

	UpdateCharacterMovementSettings();

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

//---Player Input Function Bindings---//
//Set up input bindings that call functions
void ANohCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANohCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANohCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRightLeft", this, &ANohCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpDown", this, &ANohCharacter::LookUpAtRate);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANohCharacter::NohJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ANohCharacter::NohJumpEnd);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ANohCharacter::NohCrouch);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ANohCharacter::NohSprint);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ANohCharacter::NohUnsprint);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ANohCharacter::NohAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ANohCharacter::NohUnaim);
	PlayerInputComponent->BindAction("Sheath/Unsheath", IE_Pressed, this, &ANohCharacter::Sheath_Unsheath);


	//PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ANohCharacter::Attack);
	//PlayerInputComponent->BindAction("WeaponSwitch", IE_Pressed, this, &ANohCharacter::WeaponSwitchHold);
	//PlayerInputComponent->BindAction("WeaponSwitch", IE_Released, this, &ANohCharacter::WeaponSwitchHold);
}

//---Begin Play---//
void ANohCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Create anim notify objects, called in anim sequences
	animnotifystate_pivot = NewObject<UAnimNotifyState_Pivot>(this, UAnimNotifyState_Pivot::StaticClass());

	ChangeRotationMode();
	ChangeStanceMode();
	if (stancemode == E_STANCEMODE::SM_STANDING)
	{
		UnCrouch();
	}
	else if (stancemode == E_STANCEMODE::SM_CROUCHING)
	{
		Crouch();
	}

	//Spawn katana and saya on character
	AKatana* katana{ NewObject<AKatana>(this, AKatana::StaticClass()) };
	weapon_inventory.Emplace(katana->GetWeapon());

	//hud = Cast<ANohHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

//---Tick---//
void ANohCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Roll));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Yaw));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), GetControlRotation().Pitch));
	
	if (0 == UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(GetController())))
	{
		GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetPawn()->GetActorLocation()));
	}
	else
	{
		GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation()));
	}
	

	//Calculates important variables such as if the character is moving, direction of character, if the character has movement input, the difference between the movement input and velocity, aim rate, and aim yaw delta
	CalculateEssentialVariablesTick();

	if (movementmode == E_MOVEMENTMODE::MM_GROUNDED && stancemode == E_STANCEMODE::SM_STANDING)
	{
		if (gaitmode == E_GAIT::GT_RUNNING || gaitmode == E_GAIT::GT_SPRINTING)
		{
			//Gets movementinputvelocitydifference to dynamically change acceleration and ground friction when the character changes directions, it gives it more "weight"
			//Also used for pivot system, allows time for the pivot to play before accelerating in opposite direction
			GetCharacterMovement()->MaxAcceleration = UKismetMathLibrary::MapRangeClamped(abs(movementinput_velocitydifference), 45.0f, 130.0f, 1.0f, 0.2f) * runacceleration;
			GetCharacterMovement()->GroundFriction = UKismetMathLibrary::MapRangeClamped(abs(movementinput_velocitydifference), 45.0f, 130.0f, 1.0f, 0.4f) * rungroundfriction;
		}
	}
	else if (movementmode == E_MOVEMENTMODE::MM_RAGDOLL)
	{
		//Give ragdoll more stifnmess based on the velocity, the faster the ragdoll moves the stiffer the joints
		GetMesh()->SetAllMotorsAngularDriveParams(UKismetMathLibrary::MapRangeClamped(nohcharactervelocity.Size(), 0.0f, 1000.0f, 0.0f, 25000.0f), 0.0f, 0.0f, false);

		//Disable gravity if falling too fast, prevents ragdoll from accelerating too fast and falling through the floor
		nohcharactervelocity.Z < -4000.0f ? GetMesh()->SetEnableGravity(false) : GetMesh()->SetEnableGravity(true);

		//Calculate ragdoll velocity, location, and rotation
		ragdoll_velocity = nohcharactervelocity;
		FVector ragdoll_location = GetMesh()->GetSocketLocation(FName(TEXT("pelvis")));
		FRotator ragdoll_rotation = GetMesh()->GetSocketRotation(FName(TEXT("pelvis")));
		CalculateLocRotRagdoll(ragdoll_location, ragdoll_rotation);
		//Set character location to follow ragdoll
		SetActorLocation(ragdoll_location, false, false);
		rotation_target = ragdoll_rotation;
		targetcharacter_rotationdifference = UKismetMathLibrary::NormalizedDeltaRotator(rotation_target, rotation_character).Yaw;
		rotation_character = ragdoll_rotation;
		//Set Character rotation to follow ragdoll
		SetActorRotation(rotation_character, ETeleportType::None);
	}

	//Check to see if the character wants to sprint, even when it can't. Prevents the player from having to repress the sprint key whenever sprinting is interrupted
	if (b_shouldsprint)
	{
		
		if (CanSprint())
		{
			if (!b_dooncesprint)
			{
				b_dooncesprint = true;
				EventGaitMode(E_GAIT::GT_SPRINTING);
				b_dooncerun = false;
			}
		}
		else
		{
			if (!b_dooncerun)
			{
				b_dooncerun = true;
				EventGaitMode(E_GAIT::GT_RUNNING);
				b_dooncesprint = false;
			}
		}
	}
	else
	{
		if (!b_dooncerun)
		{
			b_dooncerun = true;
			EventGaitMode(E_GAIT::GT_RUNNING);
			b_dooncesprint = false;
		}
	}

	ManageCharacterRotationTick();

	CalculateAimOffset();
	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
		speed = nohcharactervelocity.Size();
		if (b_ismoving)
		{
			CalculateGaitValue();
			CalculatePlayRates(150.0f, 350.0f, 600.0f, 150.0f);
			CalculateMovementDirection(-90.0f, 90.0f, 5.0f);
		}
		else
		{
			if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION && !IsPlayingRootMotion())
			{
				if (b_aiming)
				{
					if (stancemode == E_STANCEMODE::SM_STANDING)
					{

						TurnInPlace_Responsive(60.0f, lfturnl90, lfturnr90, 1.5f);
					}
					else if (stancemode == E_STANCEMODE::SM_CROUCHING)
					{
						TurnInPlace_Responsive(60.0f, cturnl90, cturnr90, 1.5f);
					}
				}
				else
				{
					if (!b_turninginplace)
					{
						if (stancemode == E_STANCEMODE::SM_STANDING)
						{
							TurnInPlace_Delay(100.0f, 60.0f, 0.5f, 1.5f, nturn_l_90, nturn_r_90, 130.0f, 0.0f, 1.25f, nturn_l_180, nturn_r_180);
						}
						else if (stancemode == E_STANCEMODE::SM_CROUCHING)
						{
							TurnInPlace_Delay(100.0f, 60.0f, 0.5f, 1.25f, cturnl90, cturnr90, 130.0f, 0.0f, 1.5f, cturnl90, cturnr90);
						}
					}
				}
			}
		}

		switch (activelocostate)
		{
		case E_ACTIVELOCOSTATE::ALS_NOTMOVING:
			if (b_ismoving)
			{
				CalculateStartPosition();
			}
			break;
		case E_ACTIVELOCOSTATE::ALS_MOVING:
			CalculateGroundedLeaningValues();
			break;
		case E_ACTIVELOCOSTATE::ALS_PIVOT:
			//Determine if pivot was completed or interrupted by comparing current direction to pivot direction
			//Allows locomotion to start in a different position or use a different movement direction based on whether the pivot was completed or interrupted
			bool pivotinterrupted{ FMath::IsNearlyEqual(direction, animnotifystate_pivot->pivotparams.pivotdirection, 45.0f) };
			if (pivotinterrupted)
			{
				movementdirection = animnotifystate_pivot->pivotparams.interruptedmovementdirection;
				startposition = animnotifystate_pivot->pivotparams.interruptedstarttime;
			}
			else
			{
				movementdirection = animnotifystate_pivot->pivotparams.completedmovementdirection;
				startposition = animnotifystate_pivot->pivotparams.completedstarttime;
			}
			break;
		}
		break;
	case E_MOVEMENTMODE::MM_FALLING:
		//Set speed to only use X and Y velocity values
		speed = FVector(nohcharactervelocity.X, nohcharactervelocity.Y, 0.0f).Size();
		//Use custom curve to determine when the character should start to flail using the Z velocity as the time axis
		flailblendalpha = curve_flailalpha->GetFloatValue(nohcharactervelocity.Z * -1.0f);
		//Compare current Z velocity to Jump Z velocity, then multiply with value of current speed in relation to the default run speed to get lean in air value
		leaninair = UKismetMathLibrary::MapRangeClamped(nohcharactervelocity.Z, GetCharacterMovement()->JumpZVelocity, GetCharacterMovement()->JumpZVelocity * -2.0f, 1.0, -1.0f) * UKismetMathLibrary::NormalizeToRange(speed, 0.0f, runspeed);

		CalculateLandPredictionAlpha();
		break;
	case E_MOVEMENTMODE::MM_RAGDOLL:
		//Flail Rate based on the velocity of the ragdoll. Faster the ragdoll, faster the flail
		ragdoll_flailrate = UKismetMathLibrary::MapRangeClamped(nohcharactervelocity.Size(), 0.0f, 1000.0f, 0.0f, 1.25f);
		break;
	}

	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
		if (!b_shouldfootik)
		{
			b_shouldfootik = true;
		}
		ikleftfootoffset = FMath::VInterpTo(ikleftfootoffset, IK_FootTrace(FName(TEXT("foot_l"))), GetWorld()->DeltaTimeSeconds, 15.0f);
		ikrightfootoffset = FMath::VInterpTo(ikrightfootoffset, IK_FootTrace(FName(TEXT("foot_r"))), GetWorld()->DeltaTimeSeconds, 15.0f);
		//set pelvis offset to lowest foot offset to prevent overstretching of legs
		if (ikleftfootoffset.Z < ikrightfootoffset.Z)
		{
			ikpelvisoffset = ikleftfootoffset.Z;
		}
		else
		{
			ikpelvisoffset = ikrightfootoffset.Z;
		}
		break;
	case E_MOVEMENTMODE::MM_FALLING:
		b_shouldfootik = false;
		break;
	case E_MOVEMENTMODE::MM_RAGDOLL:
		break;
	}
}

//---Essential calculations that occur every tick---//
void ANohCharacter::CalculateEssentialVariablesTick()
{
	//Determine if the character is moving, then sets 'lastvelocityrotation' and 'direction' only when moving to prevent them returning to 0 when velocity is 0
	b_ismoving = UKismetMathLibrary::NotEqual_VectorVector(FVector{ nohcharactervelocity.X, nohcharactervelocity.Y, 0.0f }, FVector{ 0.0f, 0.0f, 0.0f }, 1.0f);
	if (b_ismoving)
	{
		rotation_lastvelocity = UKismetMathLibrary::MakeRotFromX(nohcharactervelocity);
		direction = UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastvelocity, rotation_character).Yaw;
	}

	//Determine if there is movement input, then sets 'lastmovementinputrotation' and 'movementinput/velocitydifference' to prevent them from returning to 0 when movement input is 0
	b_hasmovementinput = UKismetMathLibrary::NotEqual_VectorVector(GetCharacterMovement()->GetLastInputVector(), FVector{ 0.0f, 0.0f, 0.0f }, 0.0001f);
	if (b_hasmovementinput)
	{
		rotation_lastmovementinput = UKismetMathLibrary::MakeRotFromX(GetCharacterMovement()->GetLastInputVector());
		movementinput_velocitydifference = UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastmovementinput, rotation_lastvelocity).Yaw;
	}

	//Get the rate the camera is turning and get yaw angle between 'lookingrotation' and 'characterrotation'
	float prevaimyaw{ rotation_looking.Yaw };
	rotation_looking = GetControlRotation();
	aimyawrate = (rotation_looking.Yaw - prevaimyaw) / GetWorld()->DeltaTimeSeconds;
	aimyawdelta = UKismetMathLibrary::NormalizedDeltaRotator(rotation_looking, rotation_character).Yaw;
}
//Calculates character rotation
void ANohCharacter::ManageCharacterRotationTick()
{
	if (movementmode == E_MOVEMENTMODE::MM_GROUNDED)
	{
		if (b_ismoving)
		{
			//Determine cardinal direction and use it to apply an offset relative to the looking rotation, allows character to stay rotated at set angles relative to movement while moving in an arbitrary direction
			FRotator targetrotationwithoffset{ LookDirectionWithOffset(5.0f, 60.0f, -60.0f, 120.0f, -120.0f, 5.0f) };

			if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
			{
				//Use last velocity rotation when in velocity direction mode to set character rotation
				SetCharacterRotation(FRotator{ 0.0f, rotation_lastvelocity.Yaw, 0.0f }, true, CalculateRotationRate(165.0f, 5.0f, 375.0f, 10.0f));
			}
			else if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION)
			{
				//Use looking direction with offset when in looking direction rotation mode to set character rotation
				float interpspeed{ b_aiming ? CalculateRotationRate(165.0f, 15.0f, 375.0f, 15.0f) : CalculateRotationRate(165.0f, 10.0f, 375.0f, 15.0f) };
				SetCharacterRotation(targetrotationwithoffset, true, interpspeed);
			}
		}
		else
		{
			if (!IsPlayingRootMotion() && rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION && b_aiming)
			{
				//Limit character's rotation when aiming or playing root motion to prevent camera from spinning around the character
				float aimyawlimit{ 90.0f };
				float interpspeed{ 15.0f };
				if (abs(aimyawdelta) > aimyawlimit)
				{
					float targetrotation{ aimyawdelta > 0.0f ? rotation_looking.Yaw - aimyawlimit : rotation_looking.Yaw + aimyawlimit };

					SetCharacterRotation(FRotator{ 0.0f, targetrotation, 0.0f }, true, interpspeed);
				}
			}
		}
	}
	else if (movementmode == E_MOVEMENTMODE::MM_FALLING)
	{
		if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
		{
			if (b_ismoving)
			{
				//Use jump rotation when in velocity direction mode while falling
				SetCharacterRotation(FRotator{ 0.0f, rotation_jump.Yaw, 0.0f }, true, 10.0f);
			}
		}
		else if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION)	
		{
			//Use looking rotation when in looking direction mode, set jump rotation to looking rotation so the character doesn't rotate when switching rotation modes mid-air
			rotation_jump = rotation_looking;
			SetCharacterRotation(FRotator{ 0.0f, rotation_jump.Yaw, 0.0f }, true, 10.0f);
		}
	}
}

//---Ragdoll Location and Rotation Calculations---//
//Calculate character location and rotation during ragdoll, includes a linetrace to offset the character location if too close to ground and corrects the capsule direction if the pelvis is inverted
void ANohCharacter::CalculateLocRotRagdoll(FVector& rag_loc, FRotator& rag_rot)
{
	FHitResult HitOut{ ForceInit };
	FVector Start{ rag_loc };
	FVector End{ rag_loc.X, rag_loc.Y, rag_loc.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };
	FCollisionQueryParams TraceParams{ FName(TEXT("character ragdoll")), false, this };
	ECollisionChannel CollisionChannel{ ECC_Visibility };

	GetWorld()->LineTraceSingleByChannel(HitOut, Start, End, CollisionChannel, TraceParams);
	b_ragdollonground = HitOut.bBlockingHit;

	//If the line trace hits the ground, add the offset
	if (b_ragdollonground)
	{
		float offset{ (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - abs(HitOut.ImpactPoint.Z - HitOut.TraceStart.Z)) + 2.0f };
		rag_loc = FVector{ rag_loc.X, rag_loc.Y, rag_loc.Z + offset };
	}

	//If ragdoll roll rotation is inverted, keep the capsule facing the correct direction
	rag_rot = rag_rot.Roll > 0.0f ? FRotator{ 0.0f, rag_rot.Yaw, 0.0f } : FRotator{ 0.0f, rag_rot.Yaw - 180.0f, 0.0f };
}

//---Timers---//
void ANohCharacter::LandedFrictionDelay()	//Sets braking friction factor to 0, after set amount of time determined by caller
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.0;
}

//---Virtual Functions From ACharacter---//
//Updates character movement mode when character movement mode is changed
void ANohCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	switch (GetCharacterMovement()->MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		EventMovementMode(E_MOVEMENTMODE::MM_GROUNDED);
		break;
	case MOVE_Falling:
		EventMovementMode(E_MOVEMENTMODE::MM_FALLING);
		break;
	}
}
//Updates braking friction factor if character lands from falling depending on if the character has movement input, than after a slight delay, set it back to 0
void ANohCharacter::Landed(const FHitResult& hit)
{
	Super::Landed(hit);

	if (b_hasmovementinput)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.5;
	}
	else
	{
		GetCharacterMovement()->BrakingFrictionFactor = 3.0;
	}
	GetWorld()->GetTimerManager().SetTimer(friction_timer, this, &ANohCharacter::LandedFrictionDelay, 0.2f);
}
//Updates character stance to crouch if character starts crouching
void ANohCharacter::OnStartCrouch(float halfheightadjust, float scaledhalfheightadjust)
{
	Super::OnStartCrouch(halfheightadjust, scaledhalfheightadjust);

	EventStanceMode(E_STANCEMODE::SM_CROUCHING);
}
//Updates character stance to uncrouch if character starts uncrouching
void ANohCharacter::OnEndCrouch(float halfheightadjust, float scaledhalfheightadjust)
{
	Super::OnEndCrouch(halfheightadjust, scaledhalfheightadjust);

	EventStanceMode(E_STANCEMODE::SM_STANDING);
}

//---Update Character Movement Settings---//
void ANohCharacter::UpdateCharacterMovementSettings()		//Updates the character's movement settings such as speed, acceleration , deceleration, and friction based on the the current gait state
{
	if (stancemode == E_STANCEMODE::SM_STANDING)
	{
		if (b_aiming)
		{
			switch (gaitmode)
			{
			case E_GAIT::GT_WALKING:
			case E_GAIT::GT_RUNNING:
				GetCharacterMovement()->MaxWalkSpeed = walkspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = walkspeed;
				break;
			case E_GAIT::GT_SPRINTING:
				GetCharacterMovement()->MaxWalkSpeed = runspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = runspeed;
				break;
			}
		}
		else
		{
			switch (gaitmode)
			{
			case E_GAIT::GT_WALKING:
				GetCharacterMovement()->MaxWalkSpeed = walkspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = walkspeed;
				break;
			case E_GAIT::GT_RUNNING:
				GetCharacterMovement()->MaxWalkSpeed = runspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = runspeed;
				break;
			case E_GAIT::GT_SPRINTING:
				GetCharacterMovement()->MaxWalkSpeed = sprintspeed;
				GetCharacterMovement()->MaxWalkSpeedCrouched = sprintspeed;
				break;
			}
		}
	}
	else if (stancemode == E_STANCEMODE::SM_CROUCHING)
	{
		switch (gaitmode)
		{
		case E_GAIT::GT_WALKING:
			GetCharacterMovement()->MaxWalkSpeed = crouchspeed - 50.0f;
			GetCharacterMovement()->MaxWalkSpeedCrouched = crouchspeed - 50.0f;
			break;
		case E_GAIT::GT_RUNNING:
			GetCharacterMovement()->MaxWalkSpeed = crouchspeed;
			GetCharacterMovement()->MaxWalkSpeedCrouched = crouchspeed;
			break;
		case E_GAIT::GT_SPRINTING:
			GetCharacterMovement()->MaxWalkSpeed = crouchspeed + 50.0f;
			GetCharacterMovement()->MaxWalkSpeedCrouched = crouchspeed + 50.0f;
			break;
		}
	}

	switch (gaitmode)
	{
	case E_GAIT::GT_WALKING:
		GetCharacterMovement()->MaxAcceleration = walkacceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = walkdeceleration;
		GetCharacterMovement()->GroundFriction = walkgroundfriction;
		break;
	case E_GAIT::GT_RUNNING:
	case E_GAIT::GT_SPRINTING:
		GetCharacterMovement()->MaxAcceleration = runacceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = rundeceleration;
		GetCharacterMovement()->GroundFriction = rungroundfriction;
		break;
	}
}

//---Character Rotation---//
//Set character rotation to target rotation
void ANohCharacter::SetCharacterRotation(FRotator targetrotation, bool interprotation, float interpspeed)
{
	rotation_target = targetrotation;
	//Get target rotation and character rotation difference
	targetcharacter_rotationdifference = UKismetMathLibrary::NormalizedDeltaRotator(rotation_target, rotation_character).Yaw;

	//Interp character rotation to target rotation if interprotation is true and interpspeed is not zero
	rotation_character = (interprotation && interpspeed != 0.0f) ? FMath::RInterpTo(rotation_character, rotation_target, GetWorld()->GetDeltaSeconds(), interpspeed) : rotation_target;

	//Set actor rotation to character rotation
	SetActorRotation(rotation_character, ETeleportType::None);
}
//Dynamically get rotation rate based on character's movement speed
float ANohCharacter::CalculateRotationRate(float slowspeed, float slowspeedrate, float fastspeed, float fastspeedrate)
{
	speed = FVector{nohcharactervelocity.X, nohcharactervelocity.Y, 0.0f}.Size();
	float interpseed{ speed > slowspeed ? UKismetMathLibrary::MapRangeUnclamped(speed, slowspeed, fastspeed, slowspeedrate, fastspeedrate) : UKismetMathLibrary::MapRangeUnclamped(speed, 0.0f, slowspeed, 0.0f, slowspeedrate) };

	//Rotation rate multiplier is used to ramp down rotation rate whenever it is set to 0, automatically returns to 1
	if (rotationratemultiplier != 1.0f)
	{
		rotationratemultiplier = FMath::Clamp(rotationratemultiplier + GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);
	}
	return FMath::Clamp(interpseed * rotationratemultiplier, 0.1f, 15.0f);
}
//Determine Cardinal Direction and use it to apply an offset relative to the lookingrotation, returns a target rotation
FRotator ANohCharacter::LookDirectionWithOffset(float offsetinterpspeed, float NEangle, float NWangle, float SEangle, float SWangle, float buffer)
{
	float value{ b_hasmovementinput ? UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastmovementinput, rotation_looking).Yaw : UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastvelocity, rotation_looking).Yaw };

	if (CardinalDirectionAngles(value, NWangle, NEangle, buffer, E_CARDINALDIRECTION::CD_NORTH))
	{
		cardinaldirection = E_CARDINALDIRECTION::CD_NORTH;
	}
	else if (CardinalDirectionAngles(value, NEangle, SEangle, buffer, E_CARDINALDIRECTION::CD_EAST))
	{
		cardinaldirection = E_CARDINALDIRECTION::CD_EAST;
	}
	else if (CardinalDirectionAngles(value, SWangle, NWangle, buffer, E_CARDINALDIRECTION::CD_WEST))
	{
		cardinaldirection = E_CARDINALDIRECTION::CD_WEST;
	}
	else
	{
		cardinaldirection = E_CARDINALDIRECTION::CD_SOUTH;
	}

	float target;
	if (b_aiming || gaitmode == E_GAIT::GT_RUNNING || gaitmode == E_GAIT::GT_SPRINTING)
	{
		switch (cardinaldirection)
		{
		case E_CARDINALDIRECTION::CD_NORTH:
			target = value;
			break;
		case E_CARDINALDIRECTION::CD_EAST:
			target = value - 90.0f;
			break;
		case E_CARDINALDIRECTION::CD_WEST:
			target = value + 90.0f;
			break;
		case E_CARDINALDIRECTION::CD_SOUTH:
			target = value > 0.0f ? value - 180.0f : value + 180.0f;
			break;
		default:
			target = 0.0f;
			break;
		}
	}
	else
	{
		target = 0.0f;
	}
	
	rotationoffset = FMath::FInterpTo(rotationoffset, target, GetWorld()->DeltaTimeSeconds, offsetinterpspeed);
	return FRotator{ 0.0f, rotation_looking.Yaw + rotationoffset, 0.0f };
}

//---Player Cardinal Direction---//
bool ANohCharacter::CardinalDirectionAngles(float value, float min, float max, float buffer, E_CARDINALDIRECTION cd)	//Allows switching between 2 ranges with a buffer zone
{
	if (cd == cardinaldirection)
	{
		return UKismetMathLibrary::InRange_FloatFloat(value, min - buffer, max + buffer, true, true);
	}
	else
	{
		return UKismetMathLibrary::InRange_FloatFloat(value, min + buffer, max - buffer, true, true);
	}
}

//---Character Mode Change---//
void ANohCharacter::ChangeMovementMode()
{
	if (movementmode == E_MOVEMENTMODE::MM_FALLING)
	{
		//If the character is falling, clear the play rates so it stops locomotion animations and disallow the character from turning in place
		n_playrate = 0.0f;
		c_playrate = 0.0f;
		b_shouldturninplace = false;
	}
	else if (movementmode == E_MOVEMENTMODE::MM_RAGDOLL)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.2f);
	}

	//Update character movement settings
	UpdateCharacterMovementSettings();

	if (prevmovementmode == E_MOVEMENTMODE::MM_GROUNDED)
	{
		//If the character WAS on the ground and is moving, set the jump rotation to the last velocity rotation. If not, set it to the character rotation
		if (b_ismoving)
		{
			rotation_jump = rotation_lastvelocity;
		}
		else
		{
			rotation_jump = rotation_character;
		}
		rotationoffset = 0.0f;
	}
	else if (prevmovementmode == E_MOVEMENTMODE::MM_RAGDOLL)
	{
		//If the character WAS ragdolling, set the jump rotation to the character rotation and update the camera
		rotation_jump = rotation_character;
		UpdateCamera(curve_cameralerp2);
	}

	if (movementmode == E_MOVEMENTMODE::MM_RAGDOLL)
	{
		//If the character is ragdolling, update the camera
		UpdateCamera(curve_cameralerp2);
	}
}
void ANohCharacter::ChangeRotationMode()
{
	//Update camera
	UpdateCamera(curve_cameralerp1);

	//If the character is moving when rotation mode changes, set the rotation rate multiplier to zero to slow drastic changes in rotation to make rotation smoother
	if (b_ismoving)
	{
		rotationratemultiplier = 0.0f;
	}
}
void ANohCharacter::ChangeStanceMode()
{
	//If the character is turning in place when stance changes, stop the animation, this keeps stance changes responsive
	if (b_turninginplace)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.3f, activeturninplacemontage);
	}

	//Update character movement settings and camera
	UpdateCharacterMovementSettings();
	UpdateCamera(curve_cameralerp2);
}
void ANohCharacter::ChangeGaitMode()
{
	//Update character movement settings and camera
	UpdateCharacterMovementSettings();
	UpdateCamera(curve_cameralerp2);
}
void ANohCharacter::ChangeAimMode()
{
	//Update character movement settings
	UpdateCharacterMovementSettings();

	//Update camera differntly depending if aiming or not
	if (b_aiming)
	{
		UpdateCamera(curve_cameralerp3);
	}
	else
	{
		UpdateCamera(curve_cameralerp1);
	}
}

//---Character Mode Events---//
void ANohCharacter::EventMovementMode(E_MOVEMENTMODE newmovement)
{
	//Set new character movement mode if different than previous movement state, also saves previous movement mode, then calls ChangeMovementMode()
	if (movementmode != newmovement)
	{
		prevmovementmode = movementmode;
		movementmode = newmovement;
		ChangeMovementMode();
	}
}
void ANohCharacter::EventRotationMode(E_ROTATIONMODE newrotation)
{
	//Set new character rotation state if different than previous rotation state, then calls ChangeRotationMode()
	if (rotationmode != newrotation)
	{
		rotationmode = newrotation;
		ChangeRotationMode();
	}
}
void ANohCharacter::EventStanceMode(E_STANCEMODE newstance)
{
	//Set new character stance state if different than previous stance state, then calls ChangeStanceMode()
	if (stancemode != newstance)
	{
		stancemode = newstance;
		ChangeStanceMode();
	}
}
void ANohCharacter::EventGaitMode(E_GAIT newgait)
{
	//Set new character gait state if different than previous gait state, then calls ChangeGaitMode()
	if (gaitmode != newgait)
	{
		gaitmode = newgait;
		ChangeGaitMode();
	}
}
void ANohCharacter::EventAimMode(bool isaiming)
{
	//Set new character aim state if different than previous aim state, then calls ChangeAimMode()
	if (b_aiming != isaiming)
	{
		b_aiming = isaiming;
		ChangeAimMode();
	}
}

//---Character Current Velocity---//
//Returns current velocity of the character depending on gait mode
FVector ANohCharacter::ChooseVelocity()
{
	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_NONE:
	case E_MOVEMENTMODE::MM_GROUNDED:
	case E_MOVEMENTMODE::MM_FALLING:
	default:
		return GetVelocity();
	case E_MOVEMENTMODE::MM_RAGDOLL:
		return GetMesh()->GetPhysicsLinearVelocity(FName(TEXT("pelvis")));
	}
}

//---Character Check Sprint---//
//Returns whether or not the character can sprint
bool ANohCharacter::CanSprint()
{
	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
	case E_MOVEMENTMODE::MM_FALLING:
		if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
		{
			return true;
		}
		else if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION)
		{
			if (b_hasmovementinput)
			{
				return b_aiming ? true : abs(UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastmovementinput, rotation_looking).Yaw) < 50.0f;
			}
			else
			{
				return true;
			}
		}
	case E_MOVEMENTMODE::MM_RAGDOLL:
	default:
		return true;
	}
}

//---Character Movement---//
void ANohCharacter::PlayerMovementInput(bool isforwardaxis)
{
	//Get current velocity of character based on the gait
	nohcharactervelocity = ChooseVelocity();

	//Get forward and right direction of character using controller rotation
	FVector ForwardDirection{ UKismetMathLibrary::GetForwardVector(FRotator{0.0f, GetControlRotation().Yaw, 0.0f}) };
	FVector RightDirection{ UKismetMathLibrary::GetRightVector(FRotator{0.0f, GetControlRotation().Yaw, 0.0f}) };

	//Apply movement input if on the ground or falling, else just apply torque if ragdolling
	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
	case E_MOVEMENTMODE::MM_FALLING:
		if (isforwardaxis)
		{
			AddMovementInput(ForwardDirection, forwardaxisvalue, false);
		}
		else
		{
			AddMovementInput(RightDirection, rightaxisvalue, false);
		}
		break;
	case E_MOVEMENTMODE::MM_RAGDOLL:
		FVector worldvector{ (ForwardDirection * forwardaxisvalue) + (RightDirection * rightaxisvalue) };
		worldvector.Normalize(0.0001f);
		FVector torquevector{ worldvector };
		switch (gaitmode)
		{
		case E_GAIT::GT_WALKING:
			torquevector *= 500.0f;
			break;
		case E_GAIT::GT_RUNNING:
			torquevector *= 500.0f;
			break;
		case E_GAIT::GT_SPRINTING:
			torquevector *= 800.0f;
			break;
		}
		//Applies torque to ragdoll
		GetMesh()->AddTorqueInRadians(FVector(torquevector.Y * -1.0f, torquevector.X, torquevector.Z), FName(TEXT("pelvis")), true);
		//Apply acceleration to character
		GetCharacterMovement()->AddInputVector(worldvector, false);
		break;
	}
}
void ANohCharacter::MoveForward(float Value)
{
	//Character vertical movement
	if (Controller != NULL)
	{
		forwardaxisvalue = Value;
		PlayerMovementInput(true);
	}
}
void ANohCharacter::MoveRight(float Value)
{
	//Character horizontal movement
	if (Controller != NULL)
	{
		rightaxisvalue = Value;
		PlayerMovementInput(false);
	}
}

//---Character Camera Movement---//
void ANohCharacter::TurnAtRate(float Rate)
{
	//Turn the camera left or right at a set rate (horizontalcontrollersensitivity) in delta time
	AddControllerYawInput(Rate * horizontalcontrollersensitivity * GetWorld()->GetDeltaSeconds());
}
void ANohCharacter::LookUpAtRate(float Rate)
{
	//Turn the camera up or down at a set rate (verticalcontrollersensitivity) in delta time
	AddControllerPitchInput(Rate * verticalcontrollersensitivity * GetWorld()->GetDeltaSeconds());
}

//---Character Camera System---//
void ANohCharacter::UpdateCamera(UCurveFloat* lerpcurve)	//Updates the camera's armlength, lagspeed, and socket offset based on the current gait state, then move camera to target location
{
	currentcamera_armlength = CameraBoom->TargetArmLength;
	currentcamera_lagspeed = CameraBoom->CameraLagSpeed;
	currentcamera_socketoffset = CameraBoom->SocketOffset;

	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
	case E_MOVEMENTMODE::MM_FALLING:
		if (b_aiming)
		{
			targetcamera_armlength = targetcamera_aiming_armlength;
			targetcamera_lagspeed = targetcamera_aiming_lagspeed;
			targetcamera_socketoffset = targetcamera_aiming_socketoffset;
		}
		else
		{
			if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
			{
				if (stancemode == E_STANCEMODE::SM_STANDING)
				{
					switch (gaitmode)
					{
					case E_GAIT::GT_WALKING:
						targetcamera_armlength = targetcamera_velocitydirection_walk_armlength;
						targetcamera_lagspeed = targetcamera_velocitydirection_walk_lagspeed;
						targetcamera_socketoffset = targetcamera_velocitydirection_walk_socketoffset;
						break;
					case E_GAIT::GT_RUNNING:
						targetcamera_armlength = targetcamera_velocitydirection_run_armlength;
						targetcamera_lagspeed = targetcamera_velocitydirection_run_lagspeed;
						targetcamera_socketoffset = targetcamera_velocitydirection_run_socketoffset;
						break;
					case E_GAIT::GT_SPRINTING:
						targetcamera_armlength = targetcamera_velocitydirection_sprint_armlength;
						targetcamera_lagspeed = targetcamera_velocitydirection_sprint_lagspeed;
						targetcamera_socketoffset = targetcamera_velocitydirection_sprint_socketoffset;
						break;
					}
				}
				else if (stancemode == E_STANCEMODE::SM_CROUCHING)
				{
					targetcamera_armlength = targetcamera_velocitydirection_crouching_armlength;
					targetcamera_lagspeed = targetcamera_velocitydirection_crouching_lagspeed;
					targetcamera_socketoffset = targetcamera_velocitydirection_crouching_socketoffset;
				}
			}
			else if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION)
			{
				if (stancemode == E_STANCEMODE::SM_STANDING)
				{
					switch (gaitmode)
					{
					case E_GAIT::GT_WALKING:
						targetcamera_armlength = targetcamera_lookdirection_walk_armlength;
						targetcamera_lagspeed = targetcamera_lookdirection_walk_lagspeed;
						targetcamera_socketoffset = targetcamera_lookdirection_walk_socketoffset;
						break;
					case E_GAIT::GT_RUNNING:
						targetcamera_armlength = targetcamera_lookdirection_run_armlength;
						targetcamera_lagspeed = targetcamera_lookdirection_run_lagspeed;
						targetcamera_socketoffset = targetcamera_lookdirection_run_socketoffset;
						break;
					case E_GAIT::GT_SPRINTING:
						targetcamera_armlength = targetcamera_lookdirection_sprint_armlength;
						targetcamera_lagspeed = targetcamera_lookdirection_sprint_lagspeed;
						targetcamera_socketoffset = targetcamera_lookdirection_sprint_socketoffset;
						break;
					}
				}
				else if (stancemode == E_STANCEMODE::SM_CROUCHING)
				{
					targetcamera_armlength = targetcamera_lookdirection_crouching_armlength;
					targetcamera_lagspeed = targetcamera_lookdirection_crouching_lagspeed;
					targetcamera_socketoffset = targetcamera_lookdirection_crouching_socketoffset;
				}
			}
		}
		break;
	case E_MOVEMENTMODE::MM_RAGDOLL:
		targetcamera_armlength = targetcamera_ragdoll_armlength;
		targetcamera_lagspeed = targetcamera_ragdoll_lagspeed;
		targetcamera_socketoffset = targetcamera_ragdoll_socketoffset;
		break;
	}

	FOnTimelineFloat oncameratimelineupdatereturn;
	oncameratimelineupdatereturn.BindUFunction(this, FName(TEXT("CameraTimelineUpdateReturn")));
	cameralerptimeline->AddInterpFloat(lerpcurve, oncameratimelineupdatereturn, FName(TEXT("LerpAlpha")));
	cameralerptimeline->SetFloatCurve(lerpcurve, FName(TEXT("LerpAlpha")));
	cameralerptimeline->SetTimelineLength(lerpcurve->FloatCurve.GetLastKey().Time);
	cameralerptimeline->PlayFromStart();
}
void ANohCharacter::CameraTimelineUpdateReturn(float value)
{
	CameraBoom->TargetArmLength = FMath::Lerp(currentcamera_armlength, targetcamera_armlength, value);
	CameraBoom->CameraLagSpeed = FMath::Lerp(currentcamera_lagspeed, targetcamera_lagspeed, value);
	CameraBoom->SocketOffset = FMath::Lerp(currentcamera_socketoffset, FVector{ targetcamera_socketoffset.X, targetcamera_socketoffset.Y * (b_rightshoulderaim ? 1.0f : -1.0f),targetcamera_socketoffset.Z }, value);
}

//---Character Head Aim---//
//Calculates aim offset based on if character is in velocity direction or looking direction AND whether it has movement input
void ANohCharacter::CalculateAimOffset()
{
	if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
	{
		if (b_hasmovementinput)
		{
			//Character looks in direction of Movement Input
			FRotator lookdeltacharacter{ UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastmovementinput, rotation_character) };
			float aimoffsetinterpspeed{ UKismetMathLibrary::MapRangeClamped(abs(lookdeltacharacter.Yaw - aimoffset.X), 0.0f, 180.0f, 15.0f, 5.0f) };
			aimoffset = FMath::Vector2DInterpTo(aimoffset, FVector2D{ FMath::Clamp(lookdeltacharacter.Yaw, -90.0f, 90.0f), lookdeltacharacter.Pitch }, GetWorld()->GetDeltaSeconds(), aimoffsetinterpspeed);
		}
		else
		{
			//Reset aim offset if no movement input is being applied
			aimoffset = FMath::Vector2DInterpTo(aimoffset, FVector2D{ 0.0f, 0.0f }, GetWorld()->GetDeltaSeconds(), 4.0f);
		}
	}
	else if (rotationmode == E_ROTATIONMODE::RM_LOOKINGDIRECTION)
	{
		//Character looks in direction of Looking Rotation which is the camera
		FRotator lookdeltacharacter{ UKismetMathLibrary::NormalizedDeltaRotator(rotation_looking, rotation_character) };
		float aimoffsetinterpspeed{ UKismetMathLibrary::MapRangeClamped(abs(lookdeltacharacter.Yaw - aimoffset.X), 0.0f, 180.0f, 30.0f, 5.0f) };
		aimoffset = FMath::Vector2DInterpTo(aimoffset, FVector2D{ lookdeltacharacter.Yaw, lookdeltacharacter.Pitch }, GetWorld()->GetDeltaSeconds(), aimoffsetinterpspeed);
	}
}

//---Character Gait Value---//
//Sets gaitvalue (ranging from 1 to 3) based on current speed in relation to the other movement speeds (walk=1 , run=2, sprint=3), allows you to change walk/run/sprint/ speeds while retaining a fully weighted animation within the blendspace
void ANohCharacter::CalculateGaitValue()
{
	if (speed > runspeed)
	{
		gaitvalue = UKismetMathLibrary::MapRangeClamped(speed, runspeed, sprintspeed, 2.0f, 3.0f);
	}
	else
	{
		gaitvalue = speed > walkspeed ? UKismetMathLibrary::MapRangeClamped(speed, walkspeed, runspeed, 1.0f, 2.0f) : UKismetMathLibrary::MapRangeClamped(speed, 0.0f, walkspeed, 0.0f, 1.0f);
	}
}

//---Character Locomotion Blendspace Play Rates---//
//Parameters define the speed the animation would travel at a play rate of 1, allows the play rate of the locomotion blendspace to change dynamically for each gait so that each animation can match its corresponding speed while fully blended
void ANohCharacter::CalculatePlayRates(float walkanimspeed, float runanimspeed, float sprintanimspeed, float crouchanimspeed)
{
	float tempwalkspeed{ UKismetMathLibrary::MapRangeUnclamped(speed, 0.0f, walkanimspeed, 0.0f, 1.0f) };
	float temprunspeed{ UKismetMathLibrary::MapRangeUnclamped(speed, 0.0f, runanimspeed, 0.0f, 1.0f) };
	float tempsprintspeed{ UKismetMathLibrary::MapRangeUnclamped(speed, 0.0f, sprintanimspeed, 0.0f, 1.0f) };
	float capsuleworldscale{ GetCapsuleComponent()->GetComponentScale().Z };

	//Set and scale n_playrate based on current speed and gait value in comparison to the speed the animation was animated at
	n_playrate = gaitvalue > 2.0f ? UKismetMathLibrary::MapRangeClamped(gaitvalue, 2.0f, 3.0f, temprunspeed, tempsprintspeed) : UKismetMathLibrary::MapRangeClamped(gaitvalue, 1.0f, 2.0f, tempwalkspeed, temprunspeed);
	n_playrate /= capsuleworldscale;

	c_playrate = UKismetMathLibrary::MapRangeUnclamped(speed, 0.0f, crouchanimspeed, 0.0f, 1.0f) / capsuleworldscale;
}

//---Character Movement Direction---//
//Determine whether character should use forward or backward locomotion blendspace based on current direction
//Also included buffer so that the movement direction does not rapidly switch back and forth if the direction value is right on the threshold
void ANohCharacter::CalculateMovementDirection(float directionmin, float directionmax, float buffer)
{
	bool b_forwarddirection;
	if (movementdirection == E_MOVEMENTDIRECTION::MD_FORWARDS)
	{
		b_forwarddirection = UKismetMathLibrary::InRange_FloatFloat(direction, directionmin - buffer, directionmax + buffer, true, true);
	}
	else
	{
		b_forwarddirection = UKismetMathLibrary::InRange_FloatFloat(direction, directionmin + buffer, directionmax - buffer, true, true);
	}

	movementdirection = b_forwarddirection ? E_MOVEMENTDIRECTION::MD_FORWARDS : E_MOVEMENTDIRECTION::MD_BACKWARDS;
}

//---Character Turn In Place---//
//Plays animation montage for responsive turning in place
void ANohCharacter::TurnInPlace_Responsive(float aimyawlimit, UAnimMontage*& turnleftanim, UAnimMontage*& turnrightanim, float playrate)
{
	UAnimMontage* turnanim{ aimyawdelta > 0.0f ? turnrightanim : turnleftanim };

	//Increases play rate if camera is turning fast
	playrate *= UKismetMathLibrary::MapRangeClamped(abs(aimyawrate), 120.0f, 400.0f, 1.0f, 2.0f);

	//b_shouldturninplace becomes true as soon as aimyawdelta becomes greater than aimyawlimit
	b_shouldturninplace = abs(aimyawdelta) > aimyawlimit;
	if (b_shouldturninplace)
	{
		//If character is already turning in place, it interrupts and retriggers an animation if turning in the opposite direction
		if (b_turninginplace)
		{
			if ((b_turningright && !(aimyawdelta > 0.0f)) || (!b_turningright && (aimyawdelta > 0.0f)))
			{
				if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(turnanim))
				{
					GetMesh()->GetAnimInstance()->Montage_Play(turnanim, playrate, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
		}
		else
		{
			if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(turnanim))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(turnanim, playrate, EMontagePlayReturnType::MontageLength, 0.0f, true);
			}
		}
	}
}
//Plays animation montage for delayed turning in place
void ANohCharacter::TurnInPlace_Delay(float maxcamspeed, float aimyawlimit1, float delaytime1, float playrate1, UAnimMontage* turn_L_anim1, UAnimMontage* turn_R_anim1, float aimyawlimit2, float delaytime2, float playrate2, UAnimMontage* turn_L_anim2, UAnimMontage* turn_R_anim2)
{
	//This allows the camera to rotate around the character without triggering a turn that is moving faster than a certain rate
	if ((abs(aimyawrate) < maxcamspeed) && (abs(aimyawdelta) > aimyawlimit1))
	{
		//Increase counter until it passes delay time limit which is scaled between the two aim yaw limits
		turninplacedelaycount += GetWorld()->GetDeltaSeconds();
		b_shouldturninplace = turninplacedelaycount > UKismetMathLibrary::MapRangeClamped(abs(aimyawdelta), aimyawlimit1, aimyawlimit2, delaytime1, delaytime2);
		if (b_shouldturninplace)
		{
			//Find out which montage to use
			UAnimMontage* montage;
			if (abs(aimyawdelta) >= aimyawlimit2)
			{
				montage = aimyawdelta > 0.0f ? turn_R_anim2 : turn_L_anim2;
			}
			else
			{
				montage = aimyawdelta > 0.0f ? turn_R_anim1 : turn_L_anim1;
			}

			if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(montage))
			{
				float playrate{ abs(aimyawdelta) >= aimyawlimit2 ? playrate2 : playrate1  };
				GetMesh()->GetAnimInstance()->Montage_Play(montage, playrate, EMontagePlayReturnType::MontageLength, 0.0f, true);
			}
		}
	}
	else
	{
		turninplacedelaycount = 0.0f;
	}
}

//---Character Animation Blendspace Start Position---//
//Get starting position for the locomotion animation based on initial moving direction, so no need for dedicated start animations
void ANohCharacter::CalculateStartPosition()
{
	if (stancemode == E_STANCEMODE::SM_STANDING)
	{
		if (b_aiming)
		{
			startposition = direction > 0.0f ? 0.187f : 1.0f;
		}
		else
		{
			startposition = direction > 0.0f ? 0.3f : 0.867f;
		}
	}
	else if (stancemode == E_STANCEMODE::SM_CROUCHING)
	{
		startposition = direction > 0.0f ? 0.25f : 0.5f;
	}
}

//---Character Leaning When Moving---//
//Calculate lean values so character leans when moving
void ANohCharacter::CalculateGroundedLeaningValues()
{
	//Get lean rotation value by getting difference between the current and previous velocity rotation, valuie is multiplied by speed in realtion to walking and running speed so the character does not lean while moving slowly
	float velocitydifference{ UKismetMathLibrary::NormalizedDeltaRotator(rotation_lastvelocity, rotation_previousvelocity).Yaw / GetWorld()->GetDeltaSeconds() };
	rotation_previousvelocity = rotation_lastvelocity;
	float leanrotation{ UKismetMathLibrary::MapRangeClamped(velocitydifference, -200.0f, 200.0f, -1.0f, 1.0f) * UKismetMathLibrary::MapRangeClamped(speed, walkspeed, runspeed, 0.0f, 1.0f) };

	//Get difference between current and previous speed to get lean acceleration
	float accelerationdifference{ (speed - previousspeed) / GetWorld()->GetDeltaSeconds() };
	previousspeed = speed;
	float leanacceleration{ UKismetMathLibrary::MapRangeClamped(speed, walkspeed, runspeed, 0.0f, 1.0f) };
	//use max acceleration and braking deceleration from the movement component in the map range clamped node so that the maximum allowed accelertaion is equal to 1 while max braking deceleration is equal to -1
	//The value is also multiplied by the speed in relation to walking and running speed sot that the character does not lean when moving slowly
	if (accelerationdifference > 0.0f)
	{
		leanacceleration *= UKismetMathLibrary::MapRangeClamped(abs(accelerationdifference), 0.0f, GetCharacterMovement()->GetMaxAcceleration(), 0.0f, 1.0f);
	}
	else
	{
		leanacceleration *= UKismetMathLibrary::MapRangeClamped(abs(accelerationdifference), 0.0f, GetCharacterMovement()->BrakingDecelerationWalking, 0.0f, -1.0f);
	}

	//Use lean rotation and lean acceleration to create a temp vector rotated around the -Z axis with direction as the angle degree, this allows a single leaning blendspace to support both rotation and accelertaion leaning in all directions
	FVector temp{ leanrotation, leanacceleration, 0.0f };
	temp = temp.RotateAngleAxis(direction, FVector{ 0.0f, 0.0f, -1.0f });
	leangrounded.X = temp.X;
	leangrounded.Y = temp.Y;
}

//---Character Ground Landing Prediction---//
//Predicts when the character is going to land to play to time the land animation
void ANohCharacter::CalculateLandPredictionAlpha()
{
	//Check to see if the character is moving down while in the air, if not reset landpredictionalpha back to zero
	if (nohcharactervelocity.Z < 0.0f)
	{
		FHitResult HitOut{ ForceInit };
		FVector Start{ GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight_WithoutHemisphere() };
		FVector temp_normalizedvelocity{ nohcharactervelocity.X, nohcharactervelocity.Y, FMath::Clamp(nohcharactervelocity.Z, -4000.0f, -200.0f) };
		temp_normalizedvelocity.Normalize(0.0001f);
		FVector End{ Start + temp_normalizedvelocity * UKismetMathLibrary::MapRangeClamped(nohcharactervelocity.Z, 0.0f, -4000.0f, 50.0f, 2000.0f) };
		ECollisionChannel CollisionChannel{ ECC_Visibility };
		FCollisionQueryParams TraceParams{ FName(TEXT("character_landpredictiontrace")), true, this };

		//FName TraceTag{ "For One Frame" };
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//TraceParams.TraceTag = TraceTag;

		//Sphere trace from the bottom of the character capsule in the direction of the velocity to check for the floor
		GetWorld()->SweepSingleByChannel(HitOut, Start, End, FQuat::Identity, CollisionChannel, FCollisionShape::MakeSphere(GetCapsuleComponent()->GetScaledCapsuleRadius()), TraceParams);

		//Checks to see if the trace hits a walkable floor, if not reset the landpredictionalpha back to zero
		if (HitOut.bBlockingHit && (HitOut.ImpactNormal.Z >= GetCharacterMovement()->GetWalkableFloorZ()))
		{
			//Use hitout time (time between TraceStart and TraceEnd) as the time axis for a custom curve, makes the character blend to a land prediction pose when nearing the ground, curve defines how it blends
			landpredictionalpha = FMath::FInterpTo(landpredictionalpha, curve_landpredictionalpha->GetFloatValue(UKismetMathLibrary::MapRangeClamped(HitOut.Time, 0.0f, 1.0f, 1.0f, 0.0f)), GetWorld()->GetDeltaSeconds(), 20.0f);
		}
		else
		{
			landpredictionalpha = FMath::FInterpTo(landpredictionalpha, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
		}
	}
	else
	{
		landpredictionalpha = FMath::FInterpTo(landpredictionalpha, 0.0f, GetWorld()->GetDeltaSeconds(), 10.0f);
	}
}

//---Character Foot Inverse Kinematics---//
FVector ANohCharacter::IK_FootTrace(const FName& footsocket)	//Calculates foot and hip offset using line trace
{
	FHitResult HitOut{ ForceInit };
	FVector Start{ GetMesh()->GetSocketLocation(footsocket).X, GetMesh()->GetSocketLocation(footsocket).Y, GetMesh()->GetComponentLocation().Z + ikfeettracelengthabovefeet };
	FVector End{ GetMesh()->GetSocketLocation(footsocket).X, GetMesh()->GetSocketLocation(footsocket).Y, GetMesh()->GetComponentLocation().Z - ikfeettracelengthbelowfeet };
	ECollisionChannel CollisionChannel{ ECC_Visibility };
	FCollisionQueryParams TraceParams{ FName(TEXT("character foottrace")), true, this };

	GetWorld()->LineTraceSingleByChannel(HitOut, Start, End, CollisionChannel, TraceParams);

	if (HitOut.bBlockingHit)
	{
		if (stancemode == E_STANCEMODE::SM_STANDING)
		{
			return FVector{ FMath::Clamp(UKismetMathLibrary::DegAtan2(HitOut.Normal.Y, HitOut.Normal.Z), ikfeetminstandingclamplimit.X, ikfeetmaxstandingclamplimit.X), FMath::Clamp(UKismetMathLibrary::DegAtan2(HitOut.Normal.X, HitOut.Normal.Z), ikfeetminstandingclamplimit.Y, ikfeetmaxstandingclamplimit.Y) * -1.0f, FMath::Clamp(HitOut.Location.Z - GetMesh()->GetComponentLocation().Z, ikfeetminstandingclamplimit.Z, ikfeetmaxstandingclamplimit.Z) };
		}
		else
		{
			return FVector{ FMath::Clamp(UKismetMathLibrary::DegAtan2(HitOut.Normal.Y, HitOut.Normal.Z), ikfeetmincrouchingclamplimit.X, ikfeetmaxcrouchingclamplimit.X), FMath::Clamp(UKismetMathLibrary::DegAtan2(HitOut.Normal.X, HitOut.Normal.Z), ikfeetmincrouchingclamplimit.Y, ikfeetmaxcrouchingclamplimit.Y) * -1.0f, FMath::Clamp(HitOut.Location.Z - GetMesh()->GetComponentLocation().Z, ikfeetmincrouchingclamplimit.Z, ikfeetmaxcrouchingclamplimit.Z) };
		}
	}
	else
	{
		return FVector{ 0.0f, 0.0f, 0.0f };
	}
}

//---Anim Notifies---//
void ANohCharacter::SetIdleEntryState(E_IDLEENTRYSTATE newstate)
{
	idleentrystate = newstate;
}
void ANohCharacter::SetAnimNotify_Footstep(USoundBase*& p_sound, USkeletalMeshComponent*& p_meshcomp, E_FOOTSTEPTYPE p_footsteptype, float p_volmulti, float p_pitchmulti, const FName& p_attachpoint)
{
	UAudioComponent* audiocomponent = UGameplayStatics::SpawnSoundAttached(p_sound, p_meshcomp, p_attachpoint, FVector{ 0.0f, 0.0f, 0.0f }, FRotator{ 0.0f, 0.0f, 0.0f }, EAttachLocation::KeepRelativeOffset, false, p_volmulti, p_pitchmulti, 0.0f, nullptr, nullptr, true);
	if (audiocomponent)
	{
		audiocomponent->SetIntParameter(FName(TEXT("FootstepType")), (int8)p_footsteptype);
	}
}
void ANohCharacter::SetPivotParams(float p_pivotdirection, E_MOVEMENTDIRECTION p_completedmovementdirection, float p_completedstarttime, E_MOVEMENTDIRECTION p_interruptedmovementdirection, float p_interruptedstarttime)
{
	animnotifystate_pivot->pivotparams.pivotdirection = p_pivotdirection;
	animnotifystate_pivot->pivotparams.completedmovementdirection = p_completedmovementdirection;
	animnotifystate_pivot->pivotparams.completedstarttime = p_completedstarttime;
	animnotifystate_pivot->pivotparams.interruptedmovementdirection = p_interruptedmovementdirection;
	animnotifystate_pivot->pivotparams.interruptedstarttime = p_interruptedstarttime;
}
void ANohCharacter::SetTurnInPlace(UAnimMontage*& p_montage, bool p_shouldturninplace, bool p_turninginplace, bool p_rightturn)
{
	activeturninplacemontage = p_montage;
	b_shouldturninplace = p_shouldturninplace;
	b_turninginplace = p_turninginplace;
	b_turningright = p_rightturn;
}
void ANohCharacter::AddCharacterRotation(FRotator p_addamount)
{
	p_addamount = UKismetMathLibrary::NegateRotator(p_addamount);
	rotation_target = UKismetMathLibrary::NormalizedDeltaRotator(rotation_target, p_addamount);
	targetcharacter_rotationdifference = UKismetMathLibrary::NormalizedDeltaRotator(rotation_target, rotation_character).Yaw;
	rotation_character = UKismetMathLibrary::NormalizedDeltaRotator(rotation_character, p_addamount);
	SetActorRotation(rotation_character, ETeleportType::None);
}
void ANohCharacter::AnimNotifyState_Sheathing_WeaponAttachDettach()
{
	if (!b_issheathed)
	{
		weapon_inventory[currentweaponindex]->AttachToComponent(GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_righthand");
	}
	else
	{
		weapon_inventory[currentweaponindex]->AttachToComponent(GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_pelvis");
	}
}
void ANohCharacter::AnimNotifyState_Sheathing_End()
{
	b_issheathing = false;
}


//---Character Rotation Mode---//
void ANohCharacter::NohFreeLook()
{
	//Charater rotation is based on velocity direction of character, can freely look around with camera without the character rotating
	EventRotationMode(E_ROTATIONMODE::RM_VELOCITYDIRECTION);
}
void ANohCharacter::NohCombatLook()
{
	//Character rotation is based on look direction of character, charater rotates based on camera's direction
	EventRotationMode(E_ROTATIONMODE::RM_LOOKINGDIRECTION);
}

//---Character Jump Skill---//
void ANohCharacter::NohJump()
{
	//Character jumps if standing, uncrouch if crouching
	if (stancemode == E_STANCEMODE::SM_STANDING)
	{
		if (!IsPlayingRootMotion())
		{
			Jump();
		}
	}
	else if (stancemode == E_STANCEMODE::SM_CROUCHING)
	{
		UnCrouch();
	}
}
void ANohCharacter::NohJumpEnd()
{
	//Character stops jumping
	StopJumping();
}

//---Character Crouch Skill---//
void ANohCharacter::NohCrouch()
{
	//Character crouches and uncrouches if on the ground
	if (movementmode == E_MOVEMENTMODE::MM_GROUNDED)
	{
		if (stancemode == E_STANCEMODE::SM_STANDING)
		{
			Crouch();
		}
		else if (stancemode == E_STANCEMODE::SM_CROUCHING)
		{
			UnCrouch();
		}
	}
}

//---Character Walk Skill---//
void ANohCharacter::NohWalk()
{
	//Character walks if running, runs if walking
	if (gaitmode == E_GAIT::GT_WALKING)
	{
		EventGaitMode(E_GAIT::GT_RUNNING);
	}
	else if (gaitmode == E_GAIT::GT_RUNNING)
	{
		EventGaitMode(E_GAIT::GT_WALKING);
	}
}

//---Character Sprint Skill---//
void ANohCharacter::NohSprint()
{
	//Character should sprint and sets gait back to running before sprinting if currently walking
	b_shouldsprint = true;
	if (gaitmode == E_GAIT::GT_WALKING)
	{
		EventGaitMode(E_GAIT::GT_RUNNING);
	}
}
void ANohCharacter::NohUnsprint()
{
	//Character should not sprint
	b_shouldsprint = false;
}

//---Character Aim Skill---//
void ANohCharacter::NohAim()
{
	//Switch character rotation mode to looking direction if it is currently velocity direction
	if (rotationmode == E_ROTATIONMODE::RM_VELOCITYDIRECTION)
	{
		EventRotationMode(E_ROTATIONMODE::RM_LOOKINGDIRECTION);
	}
	//Character camera zooms in when aiming
	EventAimMode(true);
}
void ANohCharacter::NohUnaim()
{
	//Character camera zooms out when not aiming
	EventAimMode(false);
}

//---Character Ragdoll Skill---//
void ANohCharacter::NohRagdoll()
{
	switch (movementmode)
	{
	case E_MOVEMENTMODE::MM_GROUNDED:
	case E_MOVEMENTMODE::MM_FALLING:
		//Character goes into ragdoll if not ragdolling already
		//Set character movement mode to none
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		//Character goes into ragdoll mode
		EventMovementMode(E_MOVEMENTMODE::MM_RAGDOLL);
		//Disable character capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Enable mesh collision
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Simulate physics on all bodies below pelvis
		GetMesh()->SetAllBodiesBelowSimulatePhysics(FName(TEXT("pelvis")), true, true);
		break;
	case E_MOVEMENTMODE::MM_RAGDOLL:
		//Character goes back into non-ragdoll mode if ragdolling already
		//Set character movement mode to falling or walking depending on if the ragdoll is on the ground
		if (b_ragdollonground)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			//Play get up from front or back animation montage depending on the pelvis rotation is facing down or up
			if (GetMesh()->GetSocketRotation(FName(TEXT("pelvis"))).Roll > 0.0f)
			{
				GetMesh()->GetAnimInstance()->Montage_Play(getupfromfront, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_Play(getupfromback, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
			}
		}
		else
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
		//Set character velocity to ragdoll velocity
		GetCharacterMovement()->Velocity = ragdoll_velocity;
		//Saves snapshot of the current skeletal mesh component pose for blending
		GetMesh()->GetAnimInstance()->SavePoseSnapshot(FName(TEXT("RagdollPose")));
		//Re-enable character capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//Disable mesh collision
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetMesh()->SetAllBodiesSimulatePhysics(false);
		break;
	}
}

//---Character Weapon Un/Sheath---//
void ANohCharacter::Sheath_Unsheath()
{
	if (movementmode == E_MOVEMENTMODE::MM_GROUNDED && stancemode == E_STANCEMODE::SM_STANDING && !b_issheathing)
	{
		if (b_issheathed)
		{
			b_issheathing = true;
			b_issheathed = false;

			weapon_inventory[currentweaponindex]->Unsheath();
		}
		else
		{
			b_issheathing = true;
			b_issheathed = true;

			weapon_inventory[currentweaponindex]->Sheath();
		}
	}
}

//---Getters---//
bool ANohCharacter::GetIsMoving()
{
	return b_ismoving;
}
bool ANohCharacter::GetEnemyHit()
{
	return b_enemyhit;
}

//---Setters---//
void ANohCharacter::SetActiveWeapon(FName weaponname)
{
	if (weaponname == "katana")
	{
		activeweapon = E_ACTIVEWEAPON::AW_KATANA;
	}
	else
	{
		activeweapon = E_ACTIVEWEAPON::AW_NONE;
	}
}

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Back to normal"));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), weapon_inventory.Num()));
//weapon_inventory[currentweaponindex]->debugprint();
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *weapon_inventory[currentweaponindex]->getweaponType().ToString()));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *Cast<AWeapons>(getWeapon())->getweaponType().ToString()));

//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("P: %f"), rotation_lastvelocity.Pitch));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Y: %f"), rotation_lastvelocity.Yaw));
//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("R: %f"), rotation_lastvelocity.Roll));
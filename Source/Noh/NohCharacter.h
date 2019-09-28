#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NohCharacterEnums.h"
#include "NohCharacter.generated.h"

UCLASS(config = Game)
class ANohCharacter : public ACharacter
{

	GENERATED_BODY()

private:

	//Character Self Ref//
	ACharacter* nohcharacterselfref;

	//Character Main States//
	E_CARDINALDIRECTION cardinaldirection;
	E_FOOTSTEPTYPE footsteptype;

	//Sub Character States//
	bool b_shouldsprint;

	//Character Turn In Place State//
	bool b_turningright;
	float turninplacedelaycount;

	//Character Rotation//
	float rotationoffset;
	float rotationratemultiplier;
	FRotator rotation_lastvelocity;
	FRotator rotation_previousvelocity;
	FRotator rotation_looking;
	FRotator rotation_lastmovementinput;
	FRotator rotation_target;
	FRotator rotation_character;
	FRotator rotation_jump;

	//Character Movement Speed Values//
	float previousspeed;

	//Character Movement Acceleration//
	float walkacceleration;
	float runacceleration;

	//Character Movement Deceleration//
	float walkdeceleration;
	float rundeceleration;

	//Movement Friction//
	float walkgroundfriction;
	float rungroundfriction;

	//Axis Values//
	float forwardaxisvalue;
	float rightaxisvalue;

	//Timers//
	FTimerHandle friction_timer;

	//Enemy Hit Flag//
	bool b_enemyhit;

	//Character Inventory//
	UPROPERTY()
	TArray<class AWeapons*> weapon_inventory;	//Weapon inventory, stores weapons in an array

	//Do Once//
	bool b_dooncesprint;
	bool b_dooncerun;

	//Cameras//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))	//Camera boom, a spring arm that pulls in towards the player if there is a collision
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))	//Follow camera, the main camera the player uses
	class UCameraComponent* FollowCamera;

	//Physical Animation Component//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPhysicalAnimationComponent* PhysicsAnimComponent;

	//Player Camera Sensitivity//
	float verticalcontrollersensitivity;
	float horizontalcontrollersensitivity;

	//Camera Timeline//
	UPROPERTY()
	class UTimelineComponent* cameralerptimeline;

	//Yaw Angle Difference Between Looking Rotation And Character Rotation//
	float aimyawdelta;

	//Current Camera Turn Rate//
	float aimyawrate;

	//Aim right or left shoulder//
	bool b_rightshoulderaim;

	//Current Aim Camera Settings//
	float currentcamera_armlength;
	float currentcamera_lagspeed;
	FVector currentcamera_socketoffset;

	//Target Aim Camera Settings//
	float targetcamera_armlength;
	float targetcamera_lagspeed;
	FVector targetcamera_socketoffset;

	float targetcamera_velocitydirection_walk_armlength;
	float targetcamera_velocitydirection_walk_lagspeed;
	FVector targetcamera_velocitydirection_walk_socketoffset;
	float targetcamera_velocitydirection_run_armlength;
	float targetcamera_velocitydirection_run_lagspeed;
	FVector targetcamera_velocitydirection_run_socketoffset;
	float targetcamera_velocitydirection_sprint_armlength;
	float targetcamera_velocitydirection_sprint_lagspeed;
	FVector targetcamera_velocitydirection_sprint_socketoffset;
	float targetcamera_velocitydirection_crouching_armlength;
	float targetcamera_velocitydirection_crouching_lagspeed;
	FVector targetcamera_velocitydirection_crouching_socketoffset;

	float targetcamera_lookdirection_walk_armlength;
	float targetcamera_lookdirection_walk_lagspeed;
	FVector targetcamera_lookdirection_walk_socketoffset;
	float targetcamera_lookdirection_run_armlength;
	float targetcamera_lookdirection_run_lagspeed;
	FVector targetcamera_lookdirection_run_socketoffset;
	float targetcamera_lookdirection_sprint_armlength;
	float targetcamera_lookdirection_sprint_lagspeed;
	FVector targetcamera_lookdirection_sprint_socketoffset;
	float targetcamera_lookdirection_crouching_armlength;
	float targetcamera_lookdirection_crouching_lagspeed;
	FVector targetcamera_lookdirection_crouching_socketoffset;

	float targetcamera_aiming_armlength;
	float targetcamera_aiming_lagspeed;
	FVector targetcamera_aiming_socketoffset;

	float targetcamera_ragdoll_armlength;
	float targetcamera_ragdoll_lagspeed;
	FVector targetcamera_ragdoll_socketoffset;

	//Camera Lerp Curve Assets//
	UPROPERTY()
	UCurveFloat* curve_cameralerp1;
	UPROPERTY()
	UCurveFloat* curve_cameralerp2;
	UPROPERTY()
	UCurveFloat* curve_cameralerp3;

	//Character Ragdoll//
	FVector ragdoll_velocity;
	bool b_ragdollonground;

	//Character Feet IK Trace Settings//
	float ikfeettracelengthabovefeet;
	float ikfeettracelengthbelowfeet;
	FVector ikfeetminstandingclamplimit;
	FVector ikfeetmincrouchingclamplimit;
	FVector ikfeetmaxstandingclamplimit;
	FVector ikfeetmaxcrouchingclamplimit;

	//Player Animation Montages//
	UPROPERTY()
	UAnimMontage* activeturninplacemontage;
	UPROPERTY()
	UAnimMontage* getupfromfront;
	UPROPERTY()
	UAnimMontage* getupfromback;
	UPROPERTY()
	UAnimMontage* lfturnl90;
	UPROPERTY()
	UAnimMontage* lfturnr90;
	UPROPERTY()
	UAnimMontage* cturnl90;
	UPROPERTY()
	UAnimMontage* cturnr90;
	UPROPERTY()
	UAnimMontage* nturn_l_90;
	UPROPERTY()
	UAnimMontage* nturn_r_90;
	UPROPERTY()
	UAnimMontage* nturn_l_180;
	UPROPERTY()
	UAnimMontage* nturn_r_180;

	//Anim notifies//
	UPROPERTY()
	class UAnimNotifyState_Pivot* animnotifystate_pivot;
	UPROPERTY()
	class UAnimNotifyState_TurnInPlace* animnotifystate_turninplace;

	//Anim Blend Curves//
	UPROPERTY()
	UCurveFloat* curve_flailalpha;
	UPROPERTY()
	UCurveFloat* curve_landpredictionalpha;



	//---Player Input Function Bindings---//
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	//From APawn interface, set up player key bindings to call a character function

	//---Begin Play---//
	virtual void BeginPlay() override;

	//---Tick---//
	virtual void Tick(float DeltaTime) override;

	//---Essential Calculations that occur every tick---//
	void CalculateEssentialVariablesTick();			//Main function for calculating essential variables in Tick
	void ManageCharacterRotationTick();				//Calculates character rotation every tick
	
	//---Ragdoll Location and Rotation Calculations---//
	void CalculateLocRotRagdoll(FVector& rag_loc, FRotator& rag_rot); //Called in tick whenever character is in ragdoll

	//---Timers---//
	void LandedFrictionDelay();		//Called by virtual void Landed()

	//---Virtual Functions From ACharacter---//
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;	//Event that is called whenever character movement mode is changed
	virtual void Landed(const FHitResult& hit) override;										//Event that is called when character lands from falling
	virtual void OnStartCrouch(float halfheightadjust, float scaledhalfheightadjust) override;	//Event that is called when character crouches
	virtual void OnEndCrouch(float halfheightadjust, float scaledhalfheightadjust) override;	//Event that is called when character uncrouches

	//---Player Update Movement Settings---//
	void UpdateCharacterMovementSettings();			//Called whenever character changes stance or gait or when aiming

	//---Character Rotation---//
	void SetCharacterRotation(FRotator targetrotation, bool interprotation, float interpspeed);				//Called whenever character needs to rotate to target rotation
	float CalculateRotationRate(float slowspeed, float slowspeedrate, float fastspeed, float fastspeedrate);	//Called when interp speed for SetCharacterRotation() is needed
	FRotator LookDirectionWithOffset(float offsetinterpspeed, float NEangle, float NWangle, float SEangle, float SWangle, float buffer);	//Called in ManageCharacterRotationTick(), returns a target rotation 

	//---Player Cardinal Direction---//
	bool CardinalDirectionAngles(float value, float min, float max, float buffer, E_CARDINALDIRECTION CD);	//Allows switching between 2 ranges with a buffer zone

	//---Character Mode Change---//
	void ChangeRotationMode();						//Called by EventRotationMode(), handles change in rotation mode
	void ChangeStanceMode();						//Called by EventStanceMode(), handles change in stance mode
	void ChangeGaitMode();							//Called by EventGaitMode(), handles change in gait mode
	void ChangeAimMode();							//Called by EventAimMode(), handles change in aim mode
	void ChangeMovementMode();						//Called by EventMovementMode(), handles change in movement mode

	//---Character Mode Events---//
	void EventMovementMode(E_MOVEMENTMODE newmovement);		//Event that is called when movement mode needs to be changed
	void EventRotationMode(E_ROTATIONMODE newrotation);		//Event that is called when rotation mode needs to be changed
	void EventStanceMode(E_STANCEMODE newstance);			//Event that is called when stance mode needs to be changed
	void EventGaitMode(E_GAIT newgait);						//Event that is called when gait mode needs to be changed
	void EventAimMode(bool isaiming);						//Event that is called when aim mode needs to be changed

	//---Character Current Velocity---//
	FVector ChooseVelocity();			//Called when character velocity is needed, returns current velocity of the character depending on gait mode

	//---Character Check Sprint---//
	bool CanSprint();					//Called when we need to check if the character can sprint

	//---Character Movement---//
	void PlayerMovementInput(bool isforwardaxis);	//Helper function for MoveForward() and MoveRight()
	void MoveForward(float Value);					//Called when Up or Down key is pressed
	void MoveRight(float Value);					//Called when Left or Right key is pressed

	//---Character Camera Movement---//
	void TurnAtRate(float Rate);					//Called via input to turn the camera left or right
	void LookUpAtRate(float Rate);					//Called via input to turn the camera look up or down

	//---Character Camera System---//
	void UpdateCamera(UCurveFloat* lerpcurve);		//Called when camera needs to be updated and moved to a new location
	UFUNCTION()
	void CameraTimelineUpdateReturn(float value);

	//---Character Head Aim---//
	void CalculateAimOffset();

	//---Character Gait Value---//
	void CalculateGaitValue();						//Called in Tick to get gaitvalue, used in locomotion blendspace

	//---Character Locomotion Blendspace Play Rates---//
	void CalculatePlayRates(float walkanimspeed, float runanimspeed, float sprintanimspeed, float crouchanimspeed);		//Called in Tick to get nplayrate and cplayrate, used in locomotion blendspace

	//---Character Movement Direction---//
	void CalculateMovementDirection(float directionmin, float directionmax, float buffer);		//Called in Tick to get movementdirection of character

	//---Character Turn In Place---//
	void TurnInPlace_Responsive(float aimyawlimit, UAnimMontage*& turnleftanim, UAnimMontage*& turnrightanim, float playrate);	//Called when character is turning in place and aiming
	void TurnInPlace_Delay(float maxcamspeed, float aimyawlimit1, float delaytime1, float playrate1, UAnimMontage* turn_L_anim1, UAnimMontage* turn_R_anim1, float aimyawlimit2, float delaytime2, float playrate2, UAnimMontage* turn_L_anim2, UAnimMontage* turn_R_anim2);	//Called every tick when character is in looking direction and not aiming

	//---Character Animation Blendspace Start Position---//
	void CalculateStartPosition();					//Called in Tick when character is not moving but ismoving is true, happens once before the active loco state changes to moving

	//---Character Leaning When Moving---//
	void CalculateGroundedLeaningValues();			//Called in Tick whenever character is moving, calculates lean values so character leans when moving

	//---Character Ground Landing Prediction---//
	void CalculateLandPredictionAlpha();			//Called in Tick whenever character is Falling, predicts when the character is going to land to play to time the land animation

	//---Character Foot Inverse Kinematics---//
	FVector IK_FootTrace(const FName& footsocket);		//Called every tick twice, calculates feet offset

	//---Character Rotation Mode---//
	void NohFreeLook();								//Called when character is in a exploration area, changes character rotation based on velocity direction of character
	void NohCombatLook();							//Called when character is in combat area, changes character rotation based on camera look direction

	//---Character Jump Skill---//
	void NohJump();									//Called when Jump key is pressed, handles character jumping
	void NohJumpEnd();								//Called when Jump key is released, stops character jumping

	//---Character Crouch Skill---//
	void NohCrouch();								//Called when Crouch key is pressed, handles character crouching

	//---Character Walk Skill---//
	void NohWalk();									//Called when Walk key is pressed, handles character walking

	//---Character Sprint Skill---//
	void NohSprint();									//Called when Sprint key is pressed, sets character gait to running and flags character as should sprint
	void NohUnsprint();								//Called when Sprint key is released, resets character to shouldn't be sprinting

	//---Character Aim Skill---//
	void NohAim();									//Called when Aim key is pressed, zooms in with camera
	void NohUnaim();								//Called when Aim key is released, resets camera

	//---Character Ragdoll Skill---//
	void NohRagdoll();								//Called when character needs to ragdoll

	//---Character Weapon Un/Sheath---//
	void Sheath_Unsheath();							//Called when Un/Sheath key is pressed, handles character un/sheathing weapon

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//FInterpTo values//
	int currentweaponindex; //Current weapon index in character weapon array inventory

	//HUD//
	UPROPERTY()
	class ANohHUD* hud;		//Pointer to HUD from controller in world

	//Sockets//
	UPROPERTY()
	FName weaponsocket;		//Weapon socket, used to determine which socket on the skeleton the weapon will go

protected:

	//Character Main States//
	UPROPERTY(BluePrintReadOnly)
	E_GAIT gaitmode;
	UPROPERTY(BluePrintReadOnly)
	E_MOVEMENTMODE movementmode;
	UPROPERTY(BluePrintReadOnly)
	E_MOVEMENTMODE prevmovementmode;
	UPROPERTY(BluePrintReadOnly)
	E_STANCEMODE stancemode;
	UPROPERTY(BluePrintReadOnly)
	E_ROTATIONMODE rotationmode;
	UPROPERTY(BluePrintReadWrite)
	E_MOVEMENTDIRECTION movementdirection;
	UPROPERTY(BluePrintReadOnly)
	E_IDLEENTRYSTATE idleentrystate;
	UPROPERTY(BluePrintReadWrite)
	E_ACTIVELOCOSTATE activelocostate;
	UPROPERTY(BluePrintReadOnly)
	E_ACTIVEWEAPON activeweapon;

	//Player Aim State//
	UPROPERTY(BluePrintReadOnly)
	bool b_aiming;

	//Player Movement State//
	UPROPERTY(BluePrintReadOnly)
	bool b_ismoving;
	UPROPERTY(BluePrintReadOnly)
	bool b_hasmovementinput;

	//Player Turn In Place State//
	UPROPERTY(BluePrintReadOnly)
	bool b_turninginplace;
	UPROPERTY(BluePrintReadOnly)
	bool b_shouldturninplace;

	//Player Movement Speed Values//
	UPROPERTY(BluePrintReadOnly)
	float speed;
	UPROPERTY(BluePrintReadOnly)
	float walkspeed;
	UPROPERTY(BluePrintReadOnly)
	float runspeed;
	UPROPERTY(BluePrintReadOnly)
	float sprintspeed;
	UPROPERTY(BluePrintReadOnly)
	float crouchspeed;

	//Player Current Velocity//
	UPROPERTY(BluePrintReadOnly)
	FVector nohcharactervelocity;

	//Player Movement Input and Velocity Difference//
	UPROPERTY(BluePrintReadOnly)
	float movementinput_velocitydifference;

	//Player Character Rotation and Target Rotation Difference//
	UPROPERTY(BluePrintReadOnly)
	float targetcharacter_rotationdifference;

	//Player Rotation//
	UPROPERTY(BluePrintReadOnly)
	float direction;

	//Player Head Aim Offset//
	UPROPERTY(BluePrintReadOnly)
	FVector2D aimoffset;

	//Player Gait Value//
	UPROPERTY(BluePrintReadOnly)
	float gaitvalue;

	//Player Locomotion Blendspace Play Rates//
	UPROPERTY(BluePrintReadOnly)
	float n_playrate;
	UPROPERTY(BluePrintReadOnly)
	float c_playrate;

	//Player Locomotion Blendspace Start Position//
	UPROPERTY(BluePrintReadOnly)
	float startposition;

	//Player Lean Blendspace Direction//
	UPROPERTY(BluePrintReadOnly)
	FVector2D leangrounded;

	//Character Falling Flail Blend Alpha//
	UPROPERTY(BluePrintReadOnly)
	float flailblendalpha;

	//Character Ragdoll Flail Rate//
	UPROPERTY(BluePrintReadOnly)
	float ragdoll_flailrate;

	//Character Ground Landing Prediction Blend Alpha//
	UPROPERTY(BluePrintReadOnly)
	float landpredictionalpha;

	//Character Air Leaning Value//
	UPROPERTY(BluePrintReadOnly)
	float leaninair;

	//Character Feet Position//
	UPROPERTY(BluePrintReadWrite)
	FVector2D feetposition;

	//Character Feet IK State//
	UPROPERTY(BluePrintReadOnly)
	bool b_shouldfootik;
	UPROPERTY(BluePrintReadOnly)
	FVector ikleftfootoffset;
	UPROPERTY(BluePrintReadOnly)
	FVector ikrightfootoffset;
	UPROPERTY(BluePrintReadOnly)
	float ikpelvisoffset;

	//Character Sheath State//
	UPROPERTY(BluePrintReadOnly)
	bool b_issheathed;
	UPROPERTY(BluePrintReadOnly)
	bool b_issheathing;

public:

	//---Constructor---//
	ANohCharacter();

	//Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//---Anim Notifies---//
	void SetIdleEntryState(E_IDLEENTRYSTATE newstate);
	void SetAnimNotify_Footstep(USoundBase*& p_sound, USkeletalMeshComponent*& p_meshcomp, E_FOOTSTEPTYPE p_footsteptype, float p_volmulti, float p_pitchmulti, const FName& p_attachpoint);
	void SetPivotParams(float p_pivotdirection, E_MOVEMENTDIRECTION p_completedmovementdirection, float p_completedstarttime, E_MOVEMENTDIRECTION p_interruptedmovementdirection, float p_interruptedstarttime);
	void SetTurnInPlace(UAnimMontage*& p_montage, bool p_shouldturninplace, bool p_turninginplace, bool p_rightturn);
	void AddCharacterRotation(FRotator p_addamountz);
	void AnimNotifyState_Sheathing_WeaponAttachDettach();
	void AnimNotifyState_Sheathing_End();

	//---Getters---//
	bool GetIsMoving();
	bool GetEnemyHit();

	//---Setters---//
	void SetActiveWeapon(FName weaponname);
};
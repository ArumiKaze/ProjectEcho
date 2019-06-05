#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "NohCharacter.generated.h"

UENUM(BlueprintType)
enum class E_IDLEENTRYSTATE : uint8
{
	N_IDLE,
	LF_IDLE,
	C_IDLE
};
UENUM(BlueprintType)
enum class E_GAIT : uint8
{
	GT_WALKING,
	GT_RUNNING,
	GT_SPRINTING
};
UENUM(BlueprintType)
enum class E_MOVEMENTMODE : uint8
{
	MM_NONE,
	MM_GROUNDED,
	MM_FALLING,
	MM_RAGDOLL
};
UENUM(BlueprintType)
enum class E_STANCEMODE : uint8
{
	SM_STANDING,
	SM_CROUCHING
};
UENUM(BlueprintType)
enum class E_ROTATIONMODE : uint8
{
	RM_VELOCITYDIRECTION,
	RM_LOOKINGDIRECTION
};
UENUM(BlueprintType)
enum class E_CARDINALDIRECTION : uint8
{
	CD_NORTH,
	CD_EAST,
	CD_WEST,
	CD_SOUTH
};
UENUM(BlueprintType)
enum class E_MOVEMENTDIRECTION : uint8
{
	MD_FORWARDS,
	MD_BACKWARDS
};
UENUM(BlueprintType)
enum class E_ACTIVELOCOSTATE : uint8
{
	ALS_NONE,
	ALS_NOTMOVING,
	ALS_MOVING,
	ALS_PIVOT,
	ALS_STOPPING
};
UENUM(BlueprintType)
enum class E_FOOTSTEPTYPE : uint8
{
	FST_STEP,
	FST_WALK,
	FST_RUN,
	FST_SPRINT,
	FST_PIVOT,
	FST_JUMP,
	FST_LAND
};

UCLASS(config = Game)
class ANohCharacter : public ACharacter
{

	GENERATED_BODY()

private:

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

	//Do Once//
	bool b_dooncesprint;
	bool b_dooncerun;

	//Cameras//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))	//Camera boom, a spring arm that pulls in towards the player if there is a collision
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))	//Follow camera, the main camera the player uses
	class UCameraComponent* FollowCamera;

	//Player Camera Sensitivity//
	float verticalcontrollersensitivity;
	float horizontalcontrollersensitivity;

	//Camera Timeline//
	UPROPERTY()
	class UTimelineComponent* cameralerptimeline;
	FOnTimelineFloat oncameratimelineupdatereturn;

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
	void TurnInPlace_Responsive(float aimyawlimit, UAnimMontage* turnleftanim, UAnimMontage* turnrightanim, float playrate);	//Called every tick when character is in looking direction and aiming
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
	void Sprint();									//Called when Sprint key is pressed, sets character gait to running and flags character as should sprint
	void Unsprint();								//Called when Sprint key is released, resets character to shouldn't be sprinting

	//---Character Aim Skill---//
	void NohAim();									//Called when Aim key is pressed, zooms in with camera
	void NohUnaim();								//Called when Aim key is released, resets camera

	//---Character Ragdoll Skill---//
	void NohRagdoll();								//Called when character needs to ragdoll

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Private sub character states//
	bool b_ischaining;					//State check combo chaining
	bool b_isswitching_camerazoom;		//State check camera is moving for Weaponswap
	bool b_canmove;						//State check character can move or not
	bool b_canledgetrace;				//
	float weaponswapcasttime;			//Current weapon swap cast time

	//FInterpTo values//
	int currentweaponindex; //Current weapon index in character weapon array inventory

	//
	UPROPERTY()
	class USphereComponent* spheretracerledge;
	FVector walltraceimpact;
	FVector wallnormal;
	FVector ledgeheight;

	//Timers//
	FTimerHandle combo_timer;
	FTimerHandle weaponswap_timer;
	
	//Inventory//
	UPROPERTY()
		TArray<class AWeapons*> weapon_inventory;	//Weapon inventory, stores weapons in an array

	//Player controller//
	UPROPERTY()
		APlayerController* NohController;	//Default controller

	//HUD//
	UPROPERTY()
		class ANohHUD* hud;		//Pointer to HUD from controller in world

	//Character equipable weapons//
	UPROPERTY()
		TSubclassOf<class ASword> spawnsword;				//Basic sword
	UPROPERTY()
		TSubclassOf<class AGreatsword> spawngreatsword;		//Great sword

	//Sockets//
	UPROPERTY()
		FName weaponsocket;		//Weapon socket, used to determine which socket on the skeleton the weapon will go

	void Cancel();
	
	//void Dodge();									//Called when Dodge key is pressed, handles dodging
	//void DodgeTick(float delta);					//Called in Tick, launches character every tick
	void Sheath_Unsheath();							//Called when Un/Sheath key is pressed, handles character un/sheathing weapon
	void WeaponSwitchHold();						//Called when Weaponswap key is held for a brief amount of time, calls WeaponSwitchAction() to handle weapon swapping if held long enough
	void weaponswitchtimeline(float delta);			//Called in Tick when weapon switching, moves camera to weapon swap position
	void Attack();									//Called when Attack key is pressed, handles character attacking
	void LedgeWallTrace();
	void LedgeHeightTrace();
	void LedgeLetGo();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
	FVector nohcharactervelocity;	//Variable that holds current velocity of character

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

	///////////////////////////////////////////////////////////////////////////////////////////////////


	//Main character states//
	UPROPERTY(BluePrintReadOnly)		//State check idle
		bool b_isidle;
	UPROPERTY(BluePrintReadOnly)		//State check dodge
		bool b_isdodging;
	UPROPERTY(BluePrintReadOnly)		//State check weapon sheathed
		bool b_issheathed;
	UPROPERTY(BluePrintReadOnly)		//State check in process of un/sheathing weapon
		bool b_issheathing;
	UPROPERTY(BluePrintReadOnly)		//State check weapon swap
		bool b_isswitching;
	UPROPERTY(BluePrintReadOnly)		//State check whether player is holding down weapon swap key
		bool b_ischargingswitch;
	UPROPERTY(BluePrintReadOnly)		//State check player attacking
		bool b_isattacking;
	UPROPERTY(BluePrintReadOnly)		//State check player in combat
		bool b_combatmode;
	UPROPERTY(BluePrintReadOnly)		//State check player ledge climbing
		bool b_isledgeclimbing;

	

	//Protected sub character states//
	UPROPERTY(BluePrintReadOnly)		//Stores which combo the character is on
		int combonumber;
	UPROPERTY(BluePrintReadOnly)		//State check whether weapon exists in inventory
		bool b_foundweapon;

	//UFUNCTION(BlueprintCallable)		//Called in dodge anim notify when animation ends
		//void DodgeCooldown();
	UFUNCTION(BlueprintCallable)		//Called in un/sheath weapon anim notify when animation ends
		void Sheath_UnsheathCooldown();
	UFUNCTION(BlueprintCallable)		//Called in Character Animation Blueprints, when attack animation ends
		void Combo();
	UFUNCTION(BlueprintCallable)		//Called in Character Animation Blueprints, resets combo at end of last combo attack or when player doesn't attack in a while
		void ComboReset();
	UFUNCTION(BlueprintCallable)
		void WeaponSwitchAction();			//Called in Blueprints, is used to call this function when UI is active because key presses are disabled during UI Only mode
	UFUNCTION(BlueprintCallable)
		void spawnweaponbyIndex(FName x);	//Called in Blueprints, is used to spawn weapon by looking through the inventory Array and sees if the player has the weapon stored
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

	////////////////////////////////////////////////////////////////////////////////////

	//Getters//
	UFUNCTION()
		bool getIsAttacking();		//Called by outside classes, checks whether character is currently attacking
	UFUNCTION()
		int getWeaponDamage();		//Called by outside classes, checks damage of current weapon that is equipped
	UFUNCTION(BlueprintCallable)
		AActor* getWeapon();		//Called in Blueprints, gets pointer to current weapon character has equipped



	//Public functions, called by other C++ classes//
	UFUNCTION()
		void addWeaponInventory(AWeapons* w);			//Called by weapon classes, spawns a weapon in the world and then emplaces it in the weapon inventory
};
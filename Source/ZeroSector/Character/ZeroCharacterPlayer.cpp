// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterPlayer.h"
#include "ZeroHeader/ZeroInputHeader.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "Component/Input/ZeroInputNightComponent.h"
#include "Component/ZeroUIComponent.h"
#include "Component/ZeroPlayerStatComponent.h"
#include "Player/ZeroPlayerController.h"
#include "Game/ZeroGameModeBase.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Weapon/ZeroWeaponBase.h"
#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UZeroInputConfig> InputConfigRef(TEXT("/Script/ZeroSector.ZeroInputData'/Game/Data/Input/DataAsset/DA_InputData.DA_InputData'"));
	if (InputConfigRef.Object)
	{
		InputConfig = InputConfigRef.Object;
	}
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	ProtectCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Protect Capsule Component"));
	ProtectCapsuleComp->SetupAttachment(GetMesh());

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(GetMesh(), TEXT("headSocket"));
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);

	InputComp = CreateDefaultSubobject<UZeroInputBaseComponent>(TEXT("Input Config Component"));
	UIComp = CreateDefaultSubobject<UZeroUIComponent>(TEXT("UI Component"));

	StatComp = CreateDefaultSubobject<UZeroPlayerStatComponent>(TEXT("Stat Component"));

	ChangeInputMap.Add(EDaySequence::EAfternoon, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputAfternoonMode)));
	ChangeInputMap.Add(EDaySequence::ENight, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputNightMode)));

	CurrentWeaponType = EWeaponType::ENone;
	TeamId = FGenericTeamId(0);
	ClassName = TEXT("Player");
}

void AZeroCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InputComp && InputComp->IsA(UZeroInputAfternoonComponent::StaticClass()))
	{
		//ZE_LOG(LogZeroSector, Display, TEXT("Tick 실행?"));
		InputComp->InteractBeam();
	}
}

void AZeroCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputConfig->IA_Move, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(InputConfig->IA_Look, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::DialogueInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::OperationBoardInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ProvisoInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::EnforceBoardInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Fire, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Fire);
	EnhancedInputComponent->BindAction(InputConfig->IA_ChangeWeapon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ChangeWeapon);
	EnhancedInputComponent->BindAction(InputConfig->IA_ToggleNote, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ToggleNoteDisplay);
	EnhancedInputComponent->BindAction(InputConfig->IA_Run, ETriggerEvent::Started, this, &AZeroCharacterPlayer::Run);
	EnhancedInputComponent->BindAction(InputConfig->IA_Run, ETriggerEvent::Completed, this, &AZeroCharacterPlayer::Walk);
	EnhancedInputComponent->BindAction(InputConfig->IA_Reloading, ETriggerEvent::Started, this, &AZeroCharacterPlayer::Reloading);
	EnhancedInputComponent->BindAction(InputConfig->IA_PauseMenu, ETriggerEvent::Started, this, &AZeroCharacterPlayer::PauseMenuDisplay);
	EnhancedInputComponent->BindAction(InputConfig->IA_ExcludeResearcher, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ExcludeResearcherDisplay);
	EnhancedInputComponent->BindAction(InputConfig->IA_SelectResearcher, ETriggerEvent::Started, this, &AZeroCharacterPlayer::SelectResearcherDisplay);

#if WITH_EDITOR
	EnhancedInputComponent->BindAction(InputConfig->IA_NightToAfternoon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::NightToAfternoon);
	EnhancedInputComponent->BindAction(InputConfig->IA_NightToFiveDay, ETriggerEvent::Started, this, &AZeroCharacterPlayer::NightToFiveDay);
#endif

}

FGenericTeamId AZeroCharacterPlayer::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterPlayer::SetHUDWidget(UZeroHUDWidget* InHUDWidget)
{
	HUDWidgetPtr = InHUDWidget;
	if (HUDWidgetPtr)
	{
		HUDWidgetPtr->SetMaxHp(/*StatComp->GetTotalStat().MaxHp*/1000.f);
		StatComp->OnHpChanged.AddUObject(HUDWidgetPtr, &UZeroHUDWidget::UpdateHpBar);
		HUDWidgetPtr->UpdateHpBar(/*StatComp->GetTotalStat().MaxHp*/1000.f);
	}
}

void AZeroCharacterPlayer::SetAfternoonHUDWidget(UZeroAfternoonHUDWidget* InHUDWidget)
{
	AfternoonHUDWidgetPtr = InHUDWidget;
	if (AfternoonHUDWidgetPtr)
	{
		AfternoonHUDWidgetPtr->SetMaxActivePoint(StatComp->GetMaxActivePoint());
		StatComp->OnChangedActivePoint.BindUObject(AfternoonHUDWidgetPtr, &UZeroAfternoonHUDWidget::UpdateAPBar);
		AfternoonHUDWidgetPtr->UpdateAPBar(StatComp->GetMaxActivePoint());
	}
}

void AZeroCharacterPlayer::DisplayInteractUI()
{
	UIComp->InteractUIDisplay();
}

void AZeroCharacterPlayer::CloseInteractUI()
{
	UIComp->InteractUIClose();
}

UZeroHUDWidget* AZeroCharacterPlayer::GetWeaponHUDWidget() const
{
	return HUDWidgetPtr;
}

#if WITH_EDITOR
void AZeroCharacterPlayer::NightToAfternoon()
{
	ChangeInputMode();
	StatComp->InitActivePoint();
}
void AZeroCharacterPlayer::NightToFiveDay()
{
	AZeroGameModeBase::Day = 4;
	ChangeInputMode();
	StatComp->InitActivePoint();
}
#endif

AZeroPlayerController* AZeroCharacterPlayer::GetOwnerController()
{
	return GetZeroPlayerController();
}

void AZeroCharacterPlayer::ChangeInputMode()
{
	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) return;
	
	GameMode->ChangeDay();
	if (InputComp && InputComp->IsA(UZeroInputAfternoonComponent::StaticClass()))
	{
		// 낮 -> 저녁
		SetInputByDaySequence(GameMode->GetDaySequence());
		UIComp->FadeInAndOutDisplay();
		UIComp->OnClickOperationNextButton.BindUObject(InputComp, &UZeroInputBaseComponent::SetupWeapon);
		CurrentWeaponType = InputComp->GetWeaponType();
	}
	else
	{
		// 저녁 -> 낮
		InputComp->SetUnequipWeapon();
		UIComp->FadeInAndOutDisplay();
		CurrentWeaponType = InputComp->GetWeaponType();
		SetInputByDaySequence(GameMode->GetDaySequence());
	}
}

float AZeroCharacterPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(Damage);

	return 0.0f;
}

void AZeroCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InputComp = NewObject<UZeroInputNightComponent>(this, UZeroInputNightComponent::StaticClass());
	InputComp->RegisterComponent();
}

void AZeroCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	Walk();
	SetInputNightMode();
	InputComp->SetupWeapon(EWeaponType::ERifle);
	CurrentWeaponType = InputComp->GetWeaponType();
	GetZeroPlayerController()->OnClearZombie.AddUObject(this, &AZeroCharacterPlayer::ChangeInputMode);
	GetZeroPlayerController()->OnClearZombie.AddUObject(StatComp, &UZeroPlayerStatComponent::InitActivePoint);
	GetZeroPlayerController()->OnNonClearZmobie.AddUObject(StatComp, &UZeroPlayerStatComponent::InitHealth);
	
	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode) 
	{ 
		GameMode->OnNightLocation.AddUObject(this, &AZeroCharacterPlayer::MoveNightStartLoc); 
		GameMode->OnAfternoonLocation.AddUObject(this, &AZeroCharacterPlayer::MoveAfternoonStartLoc); 
	}
}

APlayerController* AZeroCharacterPlayer::GetPlayerController() const
{
	return CastChecked<APlayerController>(GetController());
}

AZeroPlayerController* AZeroCharacterPlayer::GetZeroPlayerController() const
{
	return CastChecked<AZeroPlayerController>(GetController());
}

void AZeroCharacterPlayer::MoveNightStartLoc(const FVector& InLocation)
{
	MoveLocation = InLocation;
	FTimerHandle MoveNightHandle;
	GetWorld()->GetTimerManager().SetTimer(MoveNightHandle, [&]()
		{
			SetActorLocation(MoveLocation);
		}, 2.5f, false);
}

void AZeroCharacterPlayer::MoveAfternoonStartLoc(const FVector& InLocation)
{
	MoveLocation = InLocation;
	FTimerHandle MoveAfternoonHandle;
	GetWorld()->GetTimerManager().SetTimer(MoveAfternoonHandle, [&]()
		{
			SetActorLocation(MoveLocation);
		}, 2.5f, false);
}

void AZeroCharacterPlayer::Move(const FInputActionValue& Value)
{
	if (InputComp)
	{
		InputComp->Move(Value);
	}
}

void AZeroCharacterPlayer::Look(const FInputActionValue& Value)
{
	if (InputComp)
	{
		InputComp->Look(Value);
	}
}

void AZeroCharacterPlayer::Run()
{
	if (InputComp)
	{
		InputComp->Run();
	}
}

void AZeroCharacterPlayer::Walk()
{
	if (InputComp)
	{
		InputComp->Walk();
	}
}

void AZeroCharacterPlayer::Fire()
{
	if (InputComp)
	{
		InputComp->Fire();
	}
}

void AZeroCharacterPlayer::ChangeWeapon()
{
	if (InputComp)
	{
		InputComp->ChangeWeapon();
		CurrentWeaponType = InputComp->GetWeaponType();
		HUDWidgetPtr->UpdateCurrentWeapon(CurrentWeaponType);
	}
}

void AZeroCharacterPlayer::Reloading()
{
	if (InputComp)
	{
		InputComp->Reloading();
	}
}

void AZeroCharacterPlayer::DialogueInteract()
{
	if (InputComp)
	{
		InputComp->DialogueInteract();
		// StatComp->UseActivePoint(-10.f);
		// 이렇게 하면 행동력이 두번 쓰는걸로 적용되지만
		// 일단 모르겠으니까 이렇게 둠,,,,
	}
}

void AZeroCharacterPlayer::OperationBoardInteract()
{
	if (InputComp)
	{
		InputComp->OperationBoardInteract();
	}
}


void AZeroCharacterPlayer::ProvisoInteract()
{		
	if (InputComp)
	{
		InputComp->ProvisoInteract();
		// StatComp->UseActivePoint(-10.f);
	} 
}



void AZeroCharacterPlayer::EnforceBoardInteract()
{
	if (InputComp)
	{
		InputComp->EnforceBoardInteract();
		// StatComp->UseActivePoint(-10.f);
	}
}

void AZeroCharacterPlayer::ToggleNoteDisplay()
{
	if (InputComp)
	{
		InputComp->ToggleNote();
	}

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->NoteSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->NoteSFX);
	}
}

void AZeroCharacterPlayer::PauseMenuDisplay()
{
	if (InputComp)
	{	
		InputComp->PauseMenu();
	}

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->PauseSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->PauseSFX);
	}
}

void AZeroCharacterPlayer::ExcludeResearcherDisplay()
{
	if (InputComp)
	{
		InputComp->ExcludeResearcher();
	}
}

void AZeroCharacterPlayer::SelectResearcherDisplay()
{
	if (InputComp)
	{
		InputComp->SelectResearcher();
	}
}

void AZeroCharacterPlayer::SetInputByDaySequence(EDaySequence DaySequence)
{
	ChangeInputMap[DaySequence].ChangeInput.ExecuteIfBound();
}

void AZeroCharacterPlayer::SetInputAfternoonMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		if (InputComp)
		{
			InputComp->DestroyComponent();
			InputComp = nullptr;
		}

		InputComp = NewObject<UZeroInputAfternoonComponent>(this, UZeroInputAfternoonComponent::StaticClass());
		InputComp->RegisterComponent();
		AfternoonInputDelegate();

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Day, 0);
	}
}

void AZeroCharacterPlayer::SetInputNightMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		if (InputComp)
		{
			InputComp->DestroyComponent();
			InputComp = nullptr;
		}

		InputComp = NewObject<UZeroInputNightComponent>(this, UZeroInputNightComponent::StaticClass());
		InputComp->RegisterComponent();
		InputComp->OnPauseMenu.BindUObject(UIComp, &UZeroUIComponent::PauseMenuDisplay);


		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Night, 0);
	}
}

void AZeroCharacterPlayer::AfternoonInputDelegate()
{
	InputComp->OnOperationInteract.BindUObject(UIComp, &UZeroUIComponent::OperationInteract);
	InputComp->OnProvisoInteract.BindUObject(UIComp, &UZeroUIComponent::ProvisoInteract);
	InputComp->OnEnforceInteract.BindUObject(UIComp, &UZeroUIComponent::EnforceBoardInteract);
	InputComp->OnNoteDisplay.BindUObject(UIComp, &UZeroUIComponent::ToggleNoteDisplay);
	InputComp->OnPauseMenu.BindUObject(UIComp, &UZeroUIComponent::PauseMenuDisplay);
	InputComp->OnExcludeResearcher.BindUObject(UIComp, &UZeroUIComponent::ExcludeResearcherDisplay);
	InputComp->OnSelectResearcher.BindUObject(UIComp, &UZeroUIComponent::SelectResearcherDisplay);
}

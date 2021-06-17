#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CInventoryComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/CUserWidget_Player.h"
#include "ClickMoveController.h"
#include "InventorySystem/CItem.h"

ACPlayer::ACPlayer()
{
	//----------------------------------------------------
	//Use Tick
	//----------------------------------------------------
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//----------------------------------------------------
	//Widget
	//----------------------------------------------------

	CHelpers::GetClass<UCUserWidget_Player>(&UserWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

	//----------------------------------------------------
	//Create SceneComp
	//----------------------------------------------------
	//스프링암, Root 밑에 붙히기
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm",RootComponent);
	//스프링암 밑에 컬리전 박스 붙히기
	CHelpers::CreateComponent(this, &Box, "Box", SpringArm);
	//스프링암 밑에 카메라 붙히기
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	//----------------------------------------------------
	//Create ActorComp
	//----------------------------------------------------
	//ActorComponent
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Feet, "Feet");
	CHelpers::CreateActorComponent(this, &Inventroy, "Inventroy");
	

	//----------------------------------------------------
	//Comp Settings
	//----------------------------------------------------
	//메시 위치 조정
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//스켈레탈메쉬 넣어주기
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VE.SK_GothicKnight_VE'");
	GetMesh()->SetSkeletalMesh(mesh);

	//플레이어에 CAniminstance로 만든 ABP_CPlayer 넣기
	TSubclassOf<UAnimInstance> animinstance;
	CHelpers::GetClass<UAnimInstance>(&animinstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animinstance);

	//SpringArm Setting
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f; //스프링암 길이
	SpringArm->bDoCollisionTest = false; //스프링 암에 오브젝트
	SpringArm->bUsePawnControlRotation = true; //피치회전 가능
	SpringArm->bEnableCameraLag = true; //카메라 lag
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false;

	//Box
	Box->SetRelativeLocation(FVector(320.0f, 0.0f, 0.0f));
	Box->SetRelativeScale3D(FVector(10.0f, 1.0f, 1.0f));

	//뒤돌기, 카메라 회전 따라서 회전 금지
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//캐릭터 속도
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed(); //이동 속도
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0); //좌우 회전 속도
	GetCharacterMovement()->bOrientRotationToMovement = true; //회전보간
	GetCharacterMovement()->bConstrainToPlane = true; //이동을 네비 제한
	GetCharacterMovement()->bSnapToPlaneAtStart = true; //네비 벗어나면 가까운 네이 붙여서 시작
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//데미지 리턴 저장
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	//Abort By Damage
	//Action->AbortByDamage(); //데미지를 받으면 현재 액션 취소

	//데미지 체력감소
	Status->SubHealth(damage);

	UKismetSystemLibrary::K2_SetTimer(this, "AddPlayHealth", 2, true);

	UserWidget->Update(Status->GetHealth(), Status->GetMaxHealth(), Status->GetMana(), Status->GetMaxMana());

	//Set Dead State
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0;
	}

	//Hit Montage 재생 
	if(!State->IsSkillMode())
	State->SetHittedMode();

	//소진된 체력 리턴
	return Status->GetHealth();
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;
	UMaterialInstanceConstant* helmet;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Body.MI_GKnight_Body' ");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Helmet.MI_GKnight_Helmet'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&helmet, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Helmet.MI_GKnight_Helmet'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);
	HelmatMaterial = UMaterialInstanceDynamic::Create(helmet, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	GetMesh()->SetMaterial(2, HelmatMaterial);

	//바인딩
	State->OnStateTypeChaged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACPlayer::OnOverlapEnd);
	//Action->SetUnarmedMode(); //Unarmed로 시작

	//Widget
	UserWidget = CreateWidget<UCUserWidget_Player, AClickMoveController>(GetController<AClickMoveController>(), UserWidgetClass);
	UserWidget->Update(Status->GetHealth(), Status->GetMaxHealth(),Status->GetMana(),Status->GetMaxMana());
	UserWidget->AddToViewport();

}

void ACPlayer::AddPlayHealth()
{

	Status->AddHealth(10);
	UserWidget->Update(Status->GetHealth(), Status->GetMaxHealth(), Status->GetMana(), Status->GetMaxMana());
	if (Status->GetHealth() >= 100) UKismetSystemLibrary::K2_ClearTimer(this,"AddPlayHealth");
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SKillCool_1 < 5)
		SKillCool_1 += 1 * DeltaTime;

	if (SKillCool_2 < 5)
		SKillCool_2 += 1 * DeltaTime;

	if (SKillCool_3 < 5)
		SKillCool_3 += 1 * DeltaTime;
}

//축이벤트 함수
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//들어온 키에 맞는 함수 실행
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("IceBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnIceBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("SKill_Q", EInputEvent::IE_Pressed, this, &ACPlayer::OnSKillActionQ);
	PlayerInputComponent->BindAction("SKill_W", EInputEvent::IE_Pressed, this, &ACPlayer::OnSKillActionW);
	PlayerInputComponent->BindAction("SKill_E", EInputEvent::IE_Pressed, this, &ACPlayer::OnSKillActionE);
	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::PickUp);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->SetActorHiddenInGame(true);
}

void ACPlayer::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OtherActor->SetActorHiddenInGame(false);

}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	//화면 회전 속도
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	//화면 회전 속도
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

//캐릭터 회전
void ACPlayer::CharacterRotation()
{	
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//충돌대상, 충돌 타입을 배열로 넣음
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1); //월드스테틱 타입

	FHitResult hitResult;


	if (State->IsIdleMode())
	{
		if (!Action->IsUnarmedMode())
		{
			//마우스 위치랑 월드에 있는 3D메쉬랑 충돌체크 후 HitResult 리턴 함수
			controller->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, hitResult);

			
			FVector start = GetActorLocation();
			FRotator rotation = GetActorRotation();

			FRotator rtator = UKismetMathLibrary::FindLookAtRotation(start, hitResult.Location);
			FRotator rota(0, rtator.Yaw, 0);

			SetActorRotation(rota);
		}
	}

	
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += (ZoomSpeed * InAxis * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomRange.X, ZoomRange.Y);
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed(); //이동 속도
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed(); //이동 속도
}

void ACPlayer::OnEvade()
{
	//Idle이고 움직일 수 있는 상태인지
	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->CanMove());
	
	State->SetRollMode();
}


void ACPlayer::UseItem(ACItem* Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); //bp Event
	}
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

	switch (InNewType)
	{
		case EStateType::BackStep: Begin_BackStep(); break;
		case EStateType::Roll: Begin_Roll(); break;
		case EStateType::Hitted : Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
	
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackStep();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();

}

void ACPlayer::End_BackStep()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	State->SetIdleMode();
}

void ACPlayer::End_Roll()
{
	State->SetIdleMode();

}

void ACPlayer::OnFist()
{
	//Idle상태 일때만
	CheckFalse(State->IsIdleMode());
	
	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	//Idle상태 일때만
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	//Idle상태 일때만
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();
}

void ACPlayer::OnIceBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetIceBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnDoAction()
{
	CharacterRotation();

	Action->DoAction();
}

void ACPlayer::OnSKillActionQ()
{
	CheckTrue(Action->IsUnarmedMode());
	if (SKillCool_1 >= 5)
	{
		SKillCool_1 = 0;
		CharacterRotation();
		Action->SkillAction(0);
	}
}

void ACPlayer::OnSKillActionW()
{
	CheckTrue(Action->IsUnarmedMode());
	if (SKillCool_2 >= 5)
	{
		SKillCool_2 = 0;
		CharacterRotation();
		Action->SkillAction(1);
	}
}

void ACPlayer::OnSKillActionE()
{
	CheckTrue(Action->IsUnarmedMode());
	if (SKillCool_3 >= 5)
	{
		SKillCool_3 = 0;
		CharacterRotation();
		Action->SkillAction(2);
	}
}

void ACPlayer::Hitted()
{

	Montages->PlayHitted();
	Status->SetMove();
}

void ACPlayer::Dead()
{
	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDead();
}

void ACPlayer::End_Dead()
{
	Action->End_Dead();

	UKismetSystemLibrary::QuitGame(GetWorld(), GetController <AClickMoveController>(), EQuitPreference::Quit,false);
}

void ACPlayer::PickUp()
{
	//GetMesh()->OnComponentBeginOverlap
	TSubclassOf<ACItem> item;
	TArray<AActor*> items;
	//ACItem itme = GetOverlappingActors()
	GetOverlappingActors(items, item);
	

	for (auto it : items)
	{
		ACItem* pickitem = Cast<ACItem>(it);
		Inventroy->AddItem(pickitem);
		pickitem->SetActorHiddenInGame(true);
		pickitem->SetActorEnableCollision(false);
	}

	
	

}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	//BodyMaterial->SetVectorParameterValue("Material_A_tint", InColor);
	//LogoMaterial->SetVectorParameterValue("Material_A_tint", InColor);
	//HelmatMaterial->SetVectorParameterValue("Material_A_tint", InColor);
}

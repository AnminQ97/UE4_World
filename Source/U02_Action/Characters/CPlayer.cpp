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
	//��������, Root �ؿ� ������
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm",RootComponent);
	//�������� �ؿ� �ø��� �ڽ� ������
	CHelpers::CreateComponent(this, &Box, "Box", SpringArm);
	//�������� �ؿ� ī�޶� ������
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
	//�޽� ��ġ ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	//���̷�Ż�޽� �־��ֱ�
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VE.SK_GothicKnight_VE'");
	GetMesh()->SetSkeletalMesh(mesh);

	//�÷��̾ CAniminstance�� ���� ABP_CPlayer �ֱ�
	TSubclassOf<UAnimInstance> animinstance;
	CHelpers::GetClass<UAnimInstance>(&animinstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animinstance);

	//SpringArm Setting
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	SpringArm->TargetArmLength = 800.0f; //�������� ����
	SpringArm->bDoCollisionTest = false; //������ �Ͽ� ������Ʈ
	SpringArm->bUsePawnControlRotation = true; //��ġȸ�� ����
	SpringArm->bEnableCameraLag = true; //ī�޶� lag
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false;

	//Box
	Box->SetRelativeLocation(FVector(320.0f, 0.0f, 0.0f));
	Box->SetRelativeScale3D(FVector(10.0f, 1.0f, 1.0f));

	//�ڵ���, ī�޶� ȸ�� ���� ȸ�� ����
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//ĳ���� �ӵ�
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed(); //�̵� �ӵ�
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0); //�¿� ȸ�� �ӵ�
	GetCharacterMovement()->bOrientRotationToMovement = true; //ȸ������
	GetCharacterMovement()->bConstrainToPlane = true; //�̵��� �׺� ����
	GetCharacterMovement()->bSnapToPlaneAtStart = true; //�׺� ����� ����� ���� �ٿ��� ����
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//������ ���� ����
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	//Abort By Damage
	//Action->AbortByDamage(); //�������� ������ ���� �׼� ���

	//������ ü�°���
	Status->SubHealth(damage);

	UKismetSystemLibrary::K2_SetTimer(this, "AddPlayHealth", 2, true);

	UserWidget->Update(Status->GetHealth(), Status->GetMaxHealth(), Status->GetMana(), Status->GetMaxMana());

	//Set Dead State
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0;
	}

	//Hit Montage ��� 
	if(!State->IsSkillMode())
	State->SetHittedMode();

	//������ ü�� ����
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

	//���ε�
	State->OnStateTypeChaged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACPlayer::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACPlayer::OnOverlapEnd);
	//Action->SetUnarmedMode(); //Unarmed�� ����

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

//���̺�Ʈ �Լ�
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���� Ű�� �´� �Լ� ����
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
	//ȭ�� ȸ�� �ӵ�
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	//ȭ�� ȸ�� �ӵ�
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

//ĳ���� ȸ��
void ACPlayer::CharacterRotation()
{	
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//�浹���, �浹 Ÿ���� �迭�� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1); //���彺��ƽ Ÿ��

	FHitResult hitResult;


	if (State->IsIdleMode())
	{
		if (!Action->IsUnarmedMode())
		{
			//���콺 ��ġ�� ���忡 �ִ� 3D�޽��� �浹üũ �� HitResult ���� �Լ�
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
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed(); //�̵� �ӵ�
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed(); //�̵� �ӵ�
}

void ACPlayer::OnEvade()
{
	//Idle�̰� ������ �� �ִ� ��������
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
	//Idle���� �϶���
	CheckFalse(State->IsIdleMode());
	
	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	//Idle���� �϶���
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	//Idle���� �϶���
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

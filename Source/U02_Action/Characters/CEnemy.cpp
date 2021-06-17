#include "CEnemy.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"

ACEnemy::ACEnemy()
{
//----------------------------------------------------
//Create WidgetComp
//----------------------------------------------------
	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget",GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget",GetMesh());

//----------------------------------------------------
//Create ActorComp
//----------------------------------------------------
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	
//----------------------------------------------------
//Comp Settings
//----------------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

//���̷�Ż�޽� �־��ֱ�
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/GKnight/Meshes/SK_GothicKnight_VE.SK_GothicKnight_VE'");
	GetMesh()->SetSkeletalMesh(mesh);
	
//�÷��̾ CAniminstance�� ���� ABP_CPlayer �ֱ�
	TSubclassOf<UAnimInstance> animinstance;
	CHelpers::GetClass<UAnimInstance>(&animinstance, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animinstance);

	GetCharacterMovement()->MaxWalkSpeed = Status->GetSprintSpeed(); //�̵� �ӵ�
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0); //�¿� ȸ�� �ӵ�

//Widgets
	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);//������ �������۷��� �ֱ�
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);//������ �������۷��� �ֱ�
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

}

void ACEnemy::BeginPlay()
{

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* pants;
	UMaterialInstanceConstant* helmet;
	UMaterialInstanceConstant* skrit;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&pants, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Pants.MI_GKnight_Pants'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Body.MI_GKnight_Body' ");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&helmet, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnight_Helmet.MI_GKnight_Helmet'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&skrit, "MaterialInstanceConstant'/Game/GKnight/Materials/MI_GKnightSkirt.MI_GKnightSkirt'");

	PantsMaterial = UMaterialInstanceDynamic::Create(pants, this);
	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	HelmetMaterial = UMaterialInstanceDynamic::Create(helmet, this);
	SkritMaterial = UMaterialInstanceDynamic::Create(skrit, this);

	GetMesh()->SetMaterial(0, PantsMaterial);
	GetMesh()->SetMaterial(1, BodyMaterial);
	GetMesh()->SetMaterial(2, HelmetMaterial);
	GetMesh()->SetMaterial(4, SkritMaterial);
	
	//���º��� ���ε�
	State->OnStateTypeChaged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();


	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetName());
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetControllerText(GetController()->GetName());

	//ü�°�����
	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(),Status->GetMaxHealth());

	NameWidget->SetVisibility(bDrawName);
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	
	BodyMaterial->SetScalarParameterValue("UserLight", State->IsHittedMode() ? 1 : 0);
	HelmetMaterial->SetScalarParameterValue("UserLight", State->IsHittedMode() ? 1 : 0);
	PantsMaterial->SetScalarParameterValue("UserLight", State->IsHittedMode() ? 1 : 0);
	SkritMaterial->SetScalarParameterValue("UserLight", State->IsHittedMode() ? 1 : 0);
}

void ACEnemy::ResetColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	ChangeColor(color);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevTypem, EStateType InNewType)
{
	//���¿� �´� �Լ� ����
	switch (InNewType)
	{
		case EStateType::Hitted: Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//������ ���� ����
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;
	
	//Abort By Damage
	Action->AbortByDamage(); //�������� ������ ���� �׼� ���

	//������ ü�°���
	Status->SubHealth(damage);

	//Set Dead State
	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return 0;
	}

	State->SetHittedMode();

	//������ ü�� ����
	return Status->GetHealth();
}

void ACEnemy::Hitted()
{
	//ü�� ���� �ݿ�
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
	
	Status->SetMove();
	
	//Play Hit Montage
	Montages->PlayHitted();

	//Hit Back �˹�
	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter(-direction * LaunchValue, true, false);

	//�� ü����
	ChangeColor(FLinearColor::Red);
	UKismetSystemLibrary::K2_SetTimer(this, "ResetColor", 1.0f, false);
}

void ACEnemy::Dead()
{
	//�״� ��Ÿ�� ���
	CheckFalse(State->IsDeadMode());

	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	Action->Dead();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Montages->PlayDead();
}

void ACEnemy::End_Dead()
{
	Action->End_Dead();
	Destroy();
}



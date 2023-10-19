// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerMod.h"

#include "CborTypes.h"
#include "SAttributeComponent.h"

void ASPowerMod::Interact_Implementation(APawn* InstigatorPawn)
{
	ApplyMod(InstigatorPawn);
}

// Sets default values
ASPowerMod::ASPowerMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	// Create and attach a Capsule Component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	// Create and attach a Static Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/ExampleContent/Meshes/SM_PotionBottle.SM_PotionBottle'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ASPowerMod::OnBeginOverlap);
	
	HealthDelta = 5.0f;
}

// Called when the game starts or when spawned
void ASPowerMod::BeginPlay()
{
	Super::BeginPlay();
	AdjustCapsuleSize();

}

void ASPowerMod::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApplyMod(OtherActor);
	
}

void ASPowerMod::ApplyMod(AActor* AffectedActor = nullptr)
{
	USAttributeComponent* AttributeComp;
	if(AffectedActor)
	{
		AttributeComp = Cast<USAttributeComponent>(AffectedActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	} else
	{
		AttributeComp = Cast<USAttributeComponent>(GetOwner()->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	
	if(AttributeComp && !AttributeComp->IsMaxHealth())
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetVisibility(false);
		AttributeComp->ApplyHealthChange(HealthDelta);
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, this, &ASPowerMod::Respawn, 10.0f);
	}
}

// Called every frame
void ASPowerMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerMod::AdjustCapsuleSize() const
{
	if (MeshComponent)
	{
		const FBoxSphereBounds Bounds = MeshComponent->GetStaticMesh()->GetBounds();
        
		// Set the capsule radius as the maximum between the X and Y extends of the bounding box
		CapsuleComponent->SetCapsuleRadius(FMath::Max(Bounds.BoxExtent.X, Bounds.BoxExtent.Y));
        
		// Set the capsule half-height based on the Z extent of the bounding box
		CapsuleComponent->SetCapsuleHalfHeight(Bounds.BoxExtent.Z);
	}
}

void ASPowerMod::Respawn()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComponent->SetVisibility(true);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	// SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnBeginOverlap);
	
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
	AudioComp->SetupAttachment(RootComponent);
	AudioComp->bAutoActivate = false;

	DamageValue = -5.0f;

	
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();


	if (FlightSound)
	{
		AudioComp->SetSound(FlightSound);
		AudioComp->Play();
	}
	
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this, &ASMagicProjectile::QuickDestroy, 5.0f);
	
}


void ASMagicProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* InstigatorActor = GetInstigator();
	
	if(OtherActor && OtherActor != GetInstigator())
	{
		SetActorEnableCollision(false);
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComp)
		{
			AttributeComp->ApplyHealthChange(DamageValue);
		}

		QuickDestroy();
	}
}


void ASMagicProjectile::QuickDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("OASMagicProjectile::QuickDestroy()"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, this->GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleVfx, this->GetActorLocation(), EffectComp->GetComponentRotation(), true);
	this->Destroy();
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


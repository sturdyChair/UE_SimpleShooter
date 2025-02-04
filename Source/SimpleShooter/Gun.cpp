// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SKeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if(Root) SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if(Mesh) Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent *AGun::GetMesh()
{
    return Mesh;
}

bool AGun::GunTrace(FHitResult &OutHit, FVector &ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if(OwnerController == nullptr) return false;

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	OwnerController->GetPlayerViewPoint(PlayerViewpointLocation, PlayerViewpointRotation);

	FVector End = PlayerViewpointLocation + PlayerViewpointRotation.Vector()*MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	ShotDirection = -PlayerViewpointRotation.Vector();
	return GetWorld()->LineTraceSingleByChannel(OutHit, PlayerViewpointLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);;
}

AController *AGun::GetOwnerController() const
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return nullptr;
	AController* OwnerController = OwnerPawn->GetController();
	return OwnerController;
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));


	FHitResult HitResult;
	FVector ShotDirection;

	bool bHit = GunTrace(HitResult, ShotDirection);
	if(bHit) 
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitResult.Location);
		
		if(HitResult.GetActor() != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, GetOwnerController(), this);
		}
	}
	
}
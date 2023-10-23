#include "TargetSphereComponent.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

const int WP_NUMBER = 5;
const float HEIGHT = 10.0f;
const float m_to_cm = 100.0f;
const float cm_to_m = 0.01f;
const FVector WP[WP_NUMBER] =
{
	{500, -500, HEIGHT},
	{500, -100, HEIGHT},
	{450, 50, HEIGHT},
	{-500, 50, HEIGHT},
	{-500, -500, HEIGHT}
};

const float SPEED_MS = 90.0f / 3.6f;
const float R_M = 10.0f;

int nextWPIndex(int index)
{
	return (index + 1) % WP_NUMBER;
}

// Sets default values for this component's properties
UTargetSphereComponent::UTargetSphereComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTargetSphereComponent::BeginPlay()
{
	Super::BeginPlay();

	ResetPosition();
    
	FInputActionKeyMapping action("ResetTarget", EKeys::Y, false, false, false, false);
    APlayerController* controller = GetWorld()->GetFirstPlayerController();

	controller->PlayerInput->AddActionMapping(action);
    controller->InputComponent->BindAction("ResetTarget", IE_Pressed, this, &UTargetSphereComponent::ResetPosition);
}

void UTargetSphereComponent::SetPosition(FVector position)
{
	GetOwner()->SetActorLocation(position * m_to_cm);
}

void UTargetSphereComponent::ResetPosition()
{
	m_currentWP = 0;
	m_flightDirection = (WP[nextWPIndex(m_currentWP)] - WP[m_currentWP]).GetUnsafeNormal();
	//SetPosition(WP[m_currentWP]);

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (ActorItr->GetName() == "BP_FlyingPawn")
        {
			SetPosition(ActorItr->GetActorLocation() * cm_to_m);
			break;
        }
    }
}

void UTargetSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto position = GetOwner()->GetActorLocation() * cm_to_m;
	auto nextPosition = WP[nextWPIndex(m_currentWP)];

	if ((FVector::Dist(position, WP[m_currentWP]) - FVector::Dist(nextPosition, WP[m_currentWP])) >= R_M)
	{
		m_currentWP = nextWPIndex(m_currentWP);
		m_flightDirection = (WP[nextWPIndex(m_currentWP)] - position).GetUnsafeNormal();
	}

	SetPosition(position + m_flightDirection * SPEED_MS * DeltaTime);
}

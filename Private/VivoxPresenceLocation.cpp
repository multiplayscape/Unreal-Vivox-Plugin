/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxPresenceLocation.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxPresence.h"
#include "VivoxPresenceSubscription.h"

#include "VivoxGlobals.h"

UVivoxPresenceLocation *UVivoxPresenceLocation::CreateVivoxPresenceLocation(IPresenceLocation *presenceLocation) {
    UVivoxPresenceLocation *vivoxPresenceLocation = NewObject<UVivoxPresenceLocation>();
    vivoxPresenceLocation->VivoxPresenceLocation = presenceLocation;

    return vivoxPresenceLocation;
}

void UVivoxPresenceLocation::BeginDestroy() {

	VivoxPresenceLocation = nullptr;

	this->UObject::BeginDestroy();

}

FString UVivoxPresenceLocation::LocationId() const {
    return this->VivoxPresenceLocation->LocationId();
}


UVivoxPresence *UVivoxPresenceLocation::CurrentPresence() const {
    return UVivoxPresence::CreateVivoxPresence((Presence*)&(this->VivoxPresenceLocation->CurrentPresence()));
}

UVivoxPresenceSubscription *UVivoxPresenceLocation::Subscription() const {
    return UVivoxPresenceSubscription::CreateVivoxPresenceSubscription((IPresenceSubscription*)&(this->VivoxPresenceLocation->Subscription()));
}
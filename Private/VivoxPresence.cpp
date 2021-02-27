/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxPresence.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxPresence *UVivoxPresence::CreateVivoxPresence(Presence *presence) {
    UVivoxPresence *vivoxPresence = NewObject<UVivoxPresence>();
    vivoxPresence->VivoxPresence = presence;

    return vivoxPresence;
}

void UVivoxPresence::BeginDestroy() {

	VivoxPresence = nullptr;

	this->UObject::BeginDestroy();

}

EVivoxPresenceStatus UVivoxPresence::CurrentStatus() const {
    switch (this->VivoxPresence->CurrentStatus()) {
        case PresenceStatus::Unavailable:
            return EVivoxPresenceStatus::Unavailable;
            break;
        case PresenceStatus::Available:
            return EVivoxPresenceStatus::Available;
            break;
        case PresenceStatus::Chat:
            return EVivoxPresenceStatus::Chat;
            break;
        case PresenceStatus::DoNotDisturb:
            return EVivoxPresenceStatus::DoNotDisturb;
            break;
        case PresenceStatus::Away:
            return EVivoxPresenceStatus::Away;
            break;
        case PresenceStatus::ExtendedAway:
            return EVivoxPresenceStatus::ExtendedAway;
            break;
        default:
            return EVivoxPresenceStatus::Unavailable;
            break;
    }
}

FString UVivoxPresence::Message() const {
    return this->VivoxPresence->Message();
}
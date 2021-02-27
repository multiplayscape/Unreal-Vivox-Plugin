/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxPresenceSubscription.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxLoginSession.h"
#include "VivoxPresenceLocation.h"
#include "VivoxGlobals.h"

UVivoxPresenceSubscription *
UVivoxPresenceSubscription::CreateVivoxPresenceSubscription(IPresenceSubscription *presenceSubscription) {
    UVivoxPresenceSubscription *vivoxPresenceSubscription = NewObject<UVivoxPresenceSubscription>();
    vivoxPresenceSubscription->VivoxPresenceSubscription = presenceSubscription;

    {
        vivoxPresenceSubscription->AfterLocationAddedHandle = vivoxPresenceSubscription->VivoxPresenceSubscription->EventAfterLocationAdded.AddLambda([vivoxPresenceSubscription](const IPresenceLocation & vivoxPresenceLocation) {
            if (vivoxPresenceSubscription->AfterLocationAdded.IsBound()) {
                vivoxPresenceSubscription->AfterLocationAdded.Broadcast(UVivoxPresenceLocation::CreateVivoxPresenceLocation((IPresenceLocation *)&vivoxPresenceLocation));
            }
        });

        vivoxPresenceSubscription->AfterLocationUpdatedHandle = vivoxPresenceSubscription->VivoxPresenceSubscription->EventAfterLocationUpdated.AddLambda([vivoxPresenceSubscription](const IPresenceLocation & vivoxPresenceLocation) {
            if (vivoxPresenceSubscription->AfterLocationUpdated.IsBound()) {
                vivoxPresenceSubscription->AfterLocationUpdated.Broadcast(UVivoxPresenceLocation::CreateVivoxPresenceLocation((IPresenceLocation *)&vivoxPresenceLocation));
            }
        });

        vivoxPresenceSubscription->BeforeLocationRemovedHandle = vivoxPresenceSubscription->VivoxPresenceSubscription->EventBeforeLocationRemoved.AddLambda([vivoxPresenceSubscription](const IPresenceLocation & vivoxPresenceLocation) {
            if (vivoxPresenceSubscription->BeforeLocationRemoved.IsBound()) {
                vivoxPresenceSubscription->BeforeLocationRemoved.Broadcast(UVivoxPresenceLocation::CreateVivoxPresenceLocation((IPresenceLocation *)&vivoxPresenceLocation));
            }
        });
    }

    return vivoxPresenceSubscription;
}

void UVivoxPresenceSubscription::BeginDestroy() {
	this->AfterLocationAdded.Clear();
	this->AfterLocationUpdated.Clear();
	this->BeforeLocationRemoved.Clear();

	if (this->VivoxPresenceSubscription != nullptr) {
		this->VivoxPresenceSubscription->EventAfterLocationAdded.Remove(this->AfterLocationAddedHandle);
		this->VivoxPresenceSubscription->EventAfterLocationUpdated.Remove(this->AfterLocationUpdatedHandle);
		this->VivoxPresenceSubscription->EventBeforeLocationRemoved.Remove(this->BeforeLocationRemovedHandle);
	}

	this->VivoxPresenceSubscription = nullptr;
	this->UObject::BeginDestroy();
}

FVivoxAccountId UVivoxPresenceSubscription::SubscribedAccount() const {
    return FVivoxAccountId().fromVivox(this->VivoxPresenceSubscription->SubscribedAccount());
}

UVivoxLoginSession *UVivoxPresenceSubscription::LoginSession() {
    return UVivoxLoginSession::CreateVivoxLoginSession((ILoginSession *)&(this->VivoxPresenceSubscription->LoginSession()));
}

TMap<FString, UVivoxPresenceLocation *> UVivoxPresenceSubscription::Locations() const {
    TMap<FString, UVivoxPresenceLocation *> locations;
    for (const TPair<FString, IPresenceLocation *> location : this->VivoxPresenceSubscription->Locations()) {
        locations.Add(location.Key, UVivoxPresenceLocation::CreateVivoxPresenceLocation(location.Value));
    }
    return locations;
}
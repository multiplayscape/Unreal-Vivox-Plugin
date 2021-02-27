/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxParticipant.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxParticipant *UVivoxParticipant::CreateVivoxParticipant(IParticipant *participant) {
    UVivoxParticipant *VivoxParticipant = NewObject<UVivoxParticipant>();
    VivoxParticipant->VivoxParticipant = participant;

    return VivoxParticipant;
}

void UVivoxParticipant::BeginDestroy() {

	VivoxParticipant = nullptr;

	this->UObject::BeginDestroy();

}

FString UVivoxParticipant::ParticipantId() const {
    return FString(this->VivoxParticipant->ParticipantId());
}

UVivoxChannelSession *UVivoxParticipant::ParentChannelSession() const {
    return UVivoxChannelSession::CreateVivoxChannelSession((IChannelSession *)&(this->VivoxParticipant->ParentChannelSession()));
}

FVivoxAccountId UVivoxParticipant::Account() const {
    return FVivoxAccountId().fromVivox(this->VivoxParticipant->Account());
}
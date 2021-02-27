/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxChannelConnectionState.h"

#include "IChannelConnectionState.h"
#include "VivoxCore.h"

#include "VivoxChannelSession.h"

#include "VivoxGlobals.h"

UVivoxChannelConnectionState *
UVivoxChannelConnectionState::CreateVivoxChannelConnectionState(IChannelConnectionState *ChannelConnectionState) {
    UVivoxChannelConnectionState *vivoxChannelConnectionState = NewObject<UVivoxChannelConnectionState>();
    vivoxChannelConnectionState->VivoxChannelConnectionState = ChannelConnectionState;

    return vivoxChannelConnectionState;
}

void UVivoxChannelConnectionState::BeginDestroy() {

	VivoxChannelConnectionState = nullptr;

	this->UObject::BeginDestroy();

}

EVivoxConnectionState UVivoxChannelConnectionState::State() const {
    switch (VivoxChannelConnectionState->State()) {
        case ConnectionState::Disconnected:
            return EVivoxConnectionState::Disconnected;
            break;
        case ConnectionState::Connecting:
            return EVivoxConnectionState::Connecting;
            break;
        case ConnectionState::Connected:
            return EVivoxConnectionState::Connected;
            break;
        case ConnectionState::Disconnecting:
            return EVivoxConnectionState::Disconnecting;
            break;
        default:
			return EVivoxConnectionState::Disconnected;
            break;
    }
}

UVivoxChannelSession *UVivoxChannelConnectionState::ChannelSession() const {
    return UVivoxChannelSession::CreateVivoxChannelSession(&(this->VivoxChannelConnectionState->ChannelSession()));
}